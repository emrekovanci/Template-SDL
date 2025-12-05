#include "PipelineReloader.hpp"

#include <SDL3/SDL_log.h>

void PipelineReloader::setReloadCallback(const std::function<void()>& callback)
{
    m_reloadCallback = callback;
}

void PipelineReloader::handleFileAction(
    efsw::WatchID watchid,
    const std::string& dir,
    const std::string& filename,
    efsw::Action action,
    std::string oldFilename
)
{
    SDL_Log("FileWatcher event detected! ID: %ld", watchid);
    SDL_Log("DIR: %s", dir.c_str());

    switch (action)
    {
        case efsw::Actions::Add:
            SDL_Log("FILE: %s has event Added", filename.c_str());
            break;
        case efsw::Actions::Delete:
            SDL_Log("FILE: %s has event Delete", filename.c_str());
            break;
        case efsw::Actions::Modified:
            SDL_Log("FILE: %s has event Modified", filename.c_str());
            break;
        case efsw::Actions::Moved:
            SDL_Log("FILE: %s has event Moved from %s", filename.c_str(), oldFilename.c_str());
            break;
    }

    m_reloadCallback();
}
