#pragma once

#include <memory>
#include <functional>

#include <uv.h>
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

#include "decibel/niceuv/UvHandle.h"

namespace decibel
{
namespace niceuv
{
class EventLoop;

class FileChangeEvent
{
    friend EventLoop;

    using ChangeCallback = std::function<void(uv_fs_event_t*, int, int)>;
public:
    explicit FileChangeEvent(const boost::filesystem::path& filepath,
                             EventLoop* loop,
                             void* ctxt = nullptr,
                             uint32_t flags = UV_FS_EVENT_RECURSIVE);
    virtual ~FileChangeEvent() = default;

    void RegisterChangeCallback(ChangeCallback change_handler);

    void Start();
    void Stop();
private:
    static void on_file_event(uv_fs_event_t* handle,
                              const char* filename,
                              int events,
                              int status);
    void ChangeEventCallback(uv_fs_event_t* handle, int events, int status);

    void* mContext;

    boost::filesystem::path mFilepath;

    std::unique_ptr<UvHandle<uv_fs_event_t>> mHandle;
    boost::optional<ChangeCallback> mChangeCallback;

    uint32_t mFlags;
};
}
}

