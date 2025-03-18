#pragma once

#include "PipelineReloader.hpp"

#include <functional>
#include <memory>

struct PipelineWatcherContext
{
    std::unique_ptr<efsw::FileWatcher> fileWatcher { nullptr };
    std::unique_ptr<PipelineReloader> pipelineReloader { nullptr };
    efsw::WatchID watchID { 0 };

    void watch(const std::function<void()>& callback);
    void unWatch() const;
};
