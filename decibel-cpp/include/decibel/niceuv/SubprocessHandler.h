
#pragma once

#include <functional>
#include <memory>
#include <uv.h>
#include <vector>

#include "UvHandle.h"

namespace decibel
{
namespace niceuv
{
class EventLoop;

class SubprocessHandler
{
    friend class EventLoop;

    template <typename T>
    friend void
    on_read(uv_stream_t* handler, ssize_t nread, const uv_buf_t* buf);
    friend void on_write(uv_write_t* req, int status);
    friend void on_alloc(uv_handle_t* req, size_t sug_size, uv_buf_t* buf);

    friend void
    on_err_read(uv_stream_t* handler, ssize_t nread, const uv_buf_t* buf);
    friend void
    on_child_exit(uv_process_t* req, int64_t exit_status, int term_signal);

public:
    typedef std::vector<char> Buffer;

    typedef std::function<void(const Buffer&)> StdOutCallback;
    typedef std::function<void(const Buffer&)> IntStdOutCallback;
    typedef std::function<void(int)> ProcessExitCallback;

    // the important option here is UV_PROCESS_DETACHED, but support the rest
    enum SubprocessOption
    {
        // Copied straight from the libuv source, cause I"m a nut
        /*
        * Set the child process' user id.
        */
        UV_PROCESS_SETUID = (1 << 0),
        /*
        * Set the child process' group id.
        */
        UV_PROCESS_SETGID = (1 << 1),
        /*
        * Do not wrap any arguments in quotes, or perform any other escaping, when
        * converting the argument list into a command line string. This option is
        * only meaningful on Windows systems. On Unix it is silently ignored.
        */
        UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS = (1 << 2),
        /*
        * Spawn the child process in a detached state - this will make it a process
        * group leader, and will effectively enable the child to keep running after
        * the parent exits. Note that the child process will still keep the
        * parent's event loop alive unless the parent process calls uv_unref() on
        * the child's process handle.
        */
        UV_PROCESS_DETACHED = (1 << 3),
        /*
        * Hide the subprocess console window that would normally be created. This
        * option is only meaningful on Windows systems. On Unix it is silently
        * ignored.
        */
        UV_PROCESS_WINDOWS_HIDE = (1 << 4),

        // Custom starry process option
        STARRY_PROCESS_FULLY_DETACH = (1 << 5)
    };
    using SubprocessOptions = std::vector<SubprocessOption>;

public:
    SubprocessHandler(EventLoop* loop,
                      const std::vector<std::string>& cmd,
                      const SubprocessOptions& options = {});

    ~SubprocessHandler();

    void RunProcess();

    void RegisterStdOutCallback(StdOutCallback cb, bool streaming = false);

    void RegisterStdErrCallback(StdOutCallback cb);

    void RegisterProcessExitCallback(ProcessExitCallback cb);

    bool Running() const;

    const std::vector<std::string>& GetCmd() const;

private:
    void ReadCalled(ssize_t nread, const uv_buf_t* buf);

    void ErrCalled(ssize_t nread, const uv_buf_t* buf);

    void ProcessExited(int status, int term_signal);

    void TrueProcessExit();

    void StreamingRead(const Buffer& buffer);

    void BufferedRead(const Buffer& buffer);

private:
    EventLoop* mEventLoop;
    std::unique_ptr<UvHandle<uv_pipe_t>> mPipeHandle;
    std::unique_ptr<UvHandle<uv_pipe_t>> mErrPipeHandle;
    uv_process_options_t mProcessOptions;
    uv_stdio_container_t mStdioContainer[3];
    UvHandle<uv_process_t> mProcessHandle;
    StdOutCallback mStdOutCb;
    StdOutCallback mStdErrCb;
    IntStdOutCallback mInternalStdOutCb;
    ProcessExitCallback mProcessExitCb;
    Buffer mStdOutBuffer;
    Buffer mStdErrBuffer;
    bool mRunning;
    bool mStreaming;
    std::vector<std::string> mCmd;
    int mExitStatus;
    bool mEOFd;
    bool mExited;
    bool mDetach;
};
}
}
