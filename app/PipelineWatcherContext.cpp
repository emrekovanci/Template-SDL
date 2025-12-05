#include "PipelineWatcherContext.hpp"

#include <filesystem>

#include <SDL3/SDL_filesystem.h>

void PipelineWatcherContext::watch(const std::function<void()>& callback)
{
    fileWatcher = std::make_unique<efsw::FileWatcher>();
    pipelineReloader = std::make_unique<PipelineReloader>();
    pipelineReloader->setReloadCallback(callback);

    auto shadersDir = std::filesystem::path(SDL_GetBasePath()) / "assets/shaders/source";
    watchID = fileWatcher->addWatch(shadersDir.string(), pipelineReloader.get(), true);
    fileWatcher->watch();
}

void PipelineWatcherContext::unWatch() const
{
    fileWatcher->removeWatch(watchID);
}
