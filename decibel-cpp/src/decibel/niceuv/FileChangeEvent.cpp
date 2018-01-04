#include "decibel/niceuv/FileChangeEvent.h"

#include "decibel/niceuv/EventLoop.h"
#include "decibel/stdlib.h"

namespace decibel
{
namespace niceuv
{
 FileChangeEvent::FileChangeEvent(const boost::filesystem::path& filepath,
                                  EventLoop* loop,
                                  void* ctxt,
                                  uint32_t flags)
    : mContext(ctxt)
    , mFilepath(filepath)
    , mChangeCallback(nullptr)
    , mFlags(flags)
 {
     mHandle = std::make_unique<UvHandle<uv_fs_event_t>>();
     mHandle->Get()->data = this;
     loop->RegisterFileChangeEvent(this);
 }

void FileChangeEvent::RegisterChangeCallback(ChangeCallback change_handler)
{
    mChangeCallback = std::move(change_handler);
}

void FileChangeEvent::Start()
{
    uv_fs_event_start(mHandle->Get(),
                      on_file_event,
                      mFilepath.string().c_str(),
                      mFlags);
}

void FileChangeEvent::Stop()
{
    uv_fs_event_stop(mHandle->Get());
}

void FileChangeEvent::on_file_event(uv_fs_event_t* handle,
                                    const char*,
                                    int events,
                                    int status)
{
    reinterpret_cast<FileChangeEvent*>(
            handle->data)->ChangeEventCallback(handle, events, status);
}

void FileChangeEvent::ChangeEventCallback(uv_fs_event_t* handle,
                                          int events,
                                          int status)
{
    if (mChangeCallback && (events & UV_CHANGE))
    {
        (*mChangeCallback)(handle, events, status);
    }
}
}
}
