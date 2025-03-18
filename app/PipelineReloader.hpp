#pragma once

#include <functional>

#include <efsw/efsw.hpp>

class PipelineReloader : public efsw::FileWatchListener
{
    std::function<void()> m_reloadCallback;

public:
    void setReloadCallback(const std::function<void()>& callback);

    void handleFileAction(
        efsw::WatchID watchid,
        const std::string& dir,
        const std::string& filename,
        efsw::Action action,
        std::string oldFilename
    ) override;
};
