#pragma once

#include <string_view>

#include <SDL3/SDL.h>
#include <SDL3_shadercross/SDL_shadercross.h>

SDL_GPUShader* compileHLSLShaderFromSource(
    SDL_GPUDevice* gpuDevice,
    std::string_view filename,
    SDL_ShaderCross_GraphicsShaderMetadata* metaData
);
