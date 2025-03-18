#include "Common.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace
{
    SDL_ShaderCross_ShaderStage getShaderStageFromFilename(std::string_view filename)
    {
        SDL_ShaderCross_ShaderStage stage {};

        if (filename.find("vert") != std::string_view::npos)
        {
            stage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX;
        }
        else if (filename.find("frag") != std::string_view::npos)
        {
            stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;
        }
        else if (filename.find("comp") != std::string_view::npos)
        {
            stage = SDL_SHADERCROSS_SHADERSTAGE_COMPUTE;
        }

        return stage;
    }

    bool loadFileContets(const std::filesystem::path& sourceFilepath, std::vector<char>& buffer)
    {
        if (auto file = std::ifstream { sourceFilepath, std::ios_base::binary })
        {
            file.seekg(0, std::ios_base::end);
            const std::ifstream::pos_type size = file.tellg();
            if (size > 0)
            {
                file.seekg(0, std::ios_base::beg);
                buffer.resize(static_cast<std::size_t>(size));
                file.read(buffer.data(), static_cast<std::streamsize>(size));
            }
            buffer.push_back('\0');
            return true;
        }

        return false;
    }
} // namespace

SDL_GPUShader* compileHLSLShaderFromSource(
    SDL_GPUDevice* gpuDevice,
    std::string_view filename,
    SDL_ShaderCross_GraphicsShaderMetadata* metaData
)
{
    std::filesystem::path shaderPath = std::filesystem::path(SDL_GetBasePath()) / "assets/shaders/source" / filename;
    std::vector<char> shaderCode;
    if (!loadFileContets(shaderPath, shaderCode))
    {
        SDL_Log("Failed to load shader file: %s", shaderPath.string().c_str());
        return nullptr;
    }

    SDL_ShaderCross_HLSL_Info shaderInfo {
        .source = shaderCode.data(),
        .entrypoint = "main",
        .include_dir = nullptr,
        .defines = nullptr,
        .shader_stage = getShaderStageFromFilename(filename),
        .enable_debug = false,
        .name = nullptr,
        .props = 0,
    };

    SDL_ShaderCross_GraphicsShaderMetadata shaderMetadata {};
    if (metaData == nullptr)
    {
        shaderMetadata.num_samplers = 0;
        shaderMetadata.num_storage_textures = 0;
        shaderMetadata.num_storage_buffers = 0;
        shaderMetadata.num_uniform_buffers = 0;
        shaderMetadata.props = 0;
    }

    return SDL_ShaderCross_CompileGraphicsShaderFromHLSL(
        gpuDevice,
        &shaderInfo,
        metaData != nullptr ? metaData : &shaderMetadata
    );
}
