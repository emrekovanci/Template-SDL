#pragma once

struct SDL_GPUDevice;
struct SDL_Window;
struct SDL_GPUGraphicsPipeline;
struct SDL_GPUBuffer;
struct PipelineWatcherContext;

struct AppContext
{
    SDL_GPUDevice* gpuDevice { nullptr };
    SDL_Window* window { nullptr };
    SDL_GPUGraphicsPipeline* graphicsPipeline { nullptr };
    SDL_GPUBuffer* vertexBuffer { nullptr };

    PipelineWatcherContext* pipelineWatcherContext { nullptr };
};
