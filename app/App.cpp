#include "AppContext.hpp"
#include "Common.hpp"
#include "PipelineWatcherContext.hpp"
#include "Vertex.hpp"

#include <array>
#include <memory>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_shadercross/SDL_shadercross.h>

namespace
{
    const std::array<Vertex, 3> vertices {
        Vertex { .x = 0.0f, .y = 0.5f, .z = 0.0f, .r = 255, .g = 0, .b = 0, .a = 255 },
        Vertex { .x = -0.5f, .y = -0.5f, .z = 0.0f, .r = 0, .g = 255, .b = 0, .a = 255 },
        Vertex { .x = 0.5f, .y = -0.5f, .z = 0.0f, .r = 0, .g = 0, .b = 255, .a = 255 },
    };

    const std::array<SDL_GPUVertexAttribute, 2> vertexAttributes {
        SDL_GPUVertexAttribute {
            .location = 0,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset = 0,
        },
        SDL_GPUVertexAttribute {
            .location = 1,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM,
            .offset = sizeof(float) * 3,
        },
    };

    SDL_AppResult loadGraphicsPipeline(void** appstate)
    {
        auto* app = static_cast<AppContext*>(*appstate);

        SDL_GPUShader* vertexShader = compileHLSLShaderFromSource(app->gpuDevice, "RawTriangle.vert", nullptr);
        if (vertexShader == nullptr)
        {
            SDL_Log("Failed to create vertex shader: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        SDL_GPUShader* fragmentShader = compileHLSLShaderFromSource(app->gpuDevice, "SolidColor.frag", nullptr);
        if (fragmentShader == nullptr)
        {
            SDL_Log("Failed to create fragment shader: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        // color target
        SDL_GPUColorTargetDescription colorTargetDesc {
            .format = SDL_GetGPUSwapchainTextureFormat(app->gpuDevice, app->window),
        };

        // vertex buffer
        SDL_GPUVertexBufferDescription vertexBufferDesc {
            .slot = 0,
            .pitch = sizeof(Vertex),
            .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
            .instance_step_rate = 0,
        };

        // pipeline create info
        SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo {
            .vertex_shader = vertexShader,
            .fragment_shader = fragmentShader,
            .vertex_input_state {
                .vertex_buffer_descriptions = &vertexBufferDesc,
                .num_vertex_buffers = 1,
                .vertex_attributes = vertexAttributes.data(),
                .num_vertex_attributes = 2,
            },
            .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
            .rasterizer_state { .fill_mode = SDL_GPU_FILLMODE_FILL },
            .target_info {
                .color_target_descriptions = &colorTargetDesc,
                .num_color_targets = 1,
            },
        };

        SDL_GPUGraphicsPipeline* graphicsPipeline = SDL_CreateGPUGraphicsPipeline(app->gpuDevice, &pipelineCreateInfo);
        if (graphicsPipeline == nullptr)
        {
            SDL_Log("Failed to create graphics pipeline: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        app->graphicsPipeline = graphicsPipeline;

        SDL_ReleaseGPUShader(app->gpuDevice, vertexShader);
        SDL_ReleaseGPUShader(app->gpuDevice, fragmentShader);

        SDL_GPUBufferCreateInfo vertexBufferCreateInfo {
            .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
            .size = sizeof(Vertex) * std::size(vertices),
        };
        SDL_GPUBuffer* vertexBuffer = SDL_CreateGPUBuffer(app->gpuDevice, &vertexBufferCreateInfo);
        if (vertexBuffer == nullptr)
        {
            SDL_Log("Failed to create vertex buffer: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }
        app->vertexBuffer = vertexBuffer;

        SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size = sizeof(Vertex) * std::size(vertices),
        };
        SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(app->gpuDevice, &transferBufferCreateInfo);
        if (transferBuffer == nullptr)
        {
            SDL_Log("Failed to create transfer buffer: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        auto* transferData = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(app->gpuDevice, transferBuffer, false));
        if (transferData == nullptr)
        {
            SDL_Log("Failed to map transfer buffer: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        SDL_memcpy(transferData, vertices.data(), sizeof(Vertex) * vertices.size());

        SDL_UnmapGPUTransferBuffer(app->gpuDevice, transferBuffer);

        SDL_GPUCommandBuffer* uploadCommandBuffer = SDL_AcquireGPUCommandBuffer(app->gpuDevice);
        if (uploadCommandBuffer == nullptr)
        {
            SDL_Log("Failed to acquire upload command buffer: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCommandBuffer);
        if (copyPass == nullptr)
        {
            SDL_Log("Failed to begin copy pass: %s", SDL_GetError());
            return SDL_APP_FAILURE;
        }

        SDL_GPUTransferBufferLocation transferBufferLocation {
            .transfer_buffer = transferBuffer,
            .offset = 0,
        };

        SDL_GPUBufferRegion vertexBufferRegion {
            .buffer = app->vertexBuffer,
            .offset = 0,
            .size = sizeof(Vertex) * std::size(vertices),
        };

        SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &vertexBufferRegion, false);
        SDL_EndGPUCopyPass(copyPass);
        SDL_SubmitGPUCommandBuffer(uploadCommandBuffer);
        SDL_ReleaseGPUTransferBuffer(app->gpuDevice, transferBuffer);

        return SDL_APP_CONTINUE;
    }
} // namespace

SDL_AppResult SDL_AppInit(void** appstate, [[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GPUDevice* gpuDevice = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
        false,
        nullptr
    );
    if (gpuDevice == nullptr)
    {
        SDL_Log("Failed to create GPU device: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("SDL Demo", 1280, 720, SDL_WINDOW_RESIZABLE);
    if (window == nullptr)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_ClaimWindowForGPUDevice(gpuDevice, window))
    {
        SDL_Log("Failed to claim window for GPU device: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    auto pipelineWatcher = std::make_unique<PipelineWatcherContext>();
    pipelineWatcher->watch([appstate]() { loadGraphicsPipeline(appstate); });

    auto app = std::make_unique<AppContext>();
    app->gpuDevice = gpuDevice;
    app->window = window;
    app->pipelineWatcherContext = pipelineWatcher.release();
    *appstate = app.release();

    return loadGraphicsPipeline(appstate);
}

SDL_AppResult SDL_AppEvent([[maybe_unused]] void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT || (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE))
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    auto* app = static_cast<AppContext*>(appstate);

    SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(app->gpuDevice);
    if (cmdbuf == nullptr)
    {
        SDL_Log("Failed to acquire GPU command buffer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GPUTexture* swapchainTexture { nullptr };
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, app->window, &swapchainTexture, nullptr, nullptr))
    {
        SDL_Log("Failed to acquire swapchain texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GPUColorTargetInfo colorTargetInfo {
        .texture = swapchainTexture,
        .clear_color = SDL_FColor { 0.0f, 0.0f, 0.0f, 1.0f },
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE,
    };

    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, nullptr);
    SDL_BindGPUGraphicsPipeline(renderPass, app->graphicsPipeline);

    SDL_GPUBufferBinding vertexBufferBinding {
        .buffer = app->vertexBuffer,
        .offset = 0,
    };

    SDL_BindGPUVertexBuffers(renderPass, 0, &vertexBufferBinding, 1);
    SDL_DrawGPUPrimitives(renderPass, std::size(vertices), 1, 0, 0);
    SDL_EndGPURenderPass(renderPass);

    SDL_SubmitGPUCommandBuffer(cmdbuf);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, [[maybe_unused]] SDL_AppResult result)
{
    auto* app = static_cast<AppContext*>(appstate);
    SDL_ReleaseGPUBuffer(app->gpuDevice, app->vertexBuffer);
    SDL_ReleaseGPUGraphicsPipeline(app->gpuDevice, app->graphicsPipeline);
    SDL_DestroyWindow(app->window);
    SDL_DestroyGPUDevice(app->gpuDevice);

    app->pipelineWatcherContext->unWatch();
}
