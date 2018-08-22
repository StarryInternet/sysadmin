
#include <algorithm>
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>

#include "decibel/niceuv/EventLoop.h"
#include "decibel/niceuv/SubprocessHandler.h"
#include "decibel/niceuv/UvStream.h"
#include "decibel/stdlib.h"

#include <log4cxx/logger.h>

namespace
{
log4cxx::LoggerPtr spLogger(log4cxx::Logger::getLogger("SubprocessHandler"));
}

namespace decibel
{
namespace niceuv
{
void on_err_read(uv_stream_t* handler, ssize_t nread, const uv_buf_t* buf)
{
    SubprocessHandler* reader =
        reinterpret_cast<SubprocessHandler*>(handler->data);
    reader->ErrCalled(nread, buf);
    free(buf->base);
}

void on_child_exit(uv_process_t* req, int64_t exit_status, int term_signal)
{
    SubprocessHandler* proc = reinterpret_cast<SubprocessHandler*>(req->data);
    proc->ProcessExited(exit_status, term_signal);
}

SubprocessHandler::SubprocessHandler(EventLoop* loop,
                                     const std::vector<std::string>& cmd,
                                     const SubprocessOptions& options)
    : mEventLoop(loop)
    , mPipeHandle(std::make_unique<UvHandle<uv_pipe_t>>())
    , mErrPipeHandle(std::make_unique<UvHandle<uv_pipe_t>>())
    , mProcessOptions()
    , mStdioContainer()
    , mProcessHandle()
    , mStdOutCb()
    , mInternalStdOutCb()
    , mProcessExitCb()
    , mStdOutBuffer()
    , mStdErrBuffer()
    , mRunning(false)
    , mStreaming(false)
    , mCmd(cmd)
    , mExitStatus(0)
    , mEOFd(false)
    , mExited(false)
    , mDetach(false)
{
    mProcessHandle.Get()->data = this;

    memset(mStdioContainer, 0, sizeof(mStdioContainer));
    memset(&mProcessOptions, 0, sizeof(mProcessOptions));

    // stdio container init, only reading stdout right now
    mStdioContainer[0].flags = UV_IGNORE;
    mStdioContainer[1].flags =
        (uv_stdio_flags)(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    mStdioContainer[2].flags =
        (uv_stdio_flags)(UV_CREATE_PIPE | UV_WRITABLE_PIPE);

    // process options init
    char** args = (char**)malloc(sizeof(char*) * (cmd.size() + 1));
    for (size_t i = 0; i < cmd.size(); i++)
    {
        args[i] = (char*)malloc(sizeof(char) * (cmd[i].length() + 1));
        strncpy(args[i], cmd[i].c_str(), cmd[i].length());
        args[i][cmd[i].length()] = '\0';
    }
    args[cmd.size()] = NULL;

    mProcessOptions.exit_cb = on_child_exit;
    mProcessOptions.file = args[0];
    mProcessOptions.args = args;
    mProcessOptions.stdio = mStdioContainer;
    mProcessOptions.stdio_count = 3;

    unsigned int flags = 0;
    std::for_each(options.begin(),
                  options.end(),
                  [this, &flags](const SubprocessOption& option)
        {
            if (option == SubprocessOption::STARRY_PROCESS_FULLY_DETACH)
            {
                return;
            }
            // Recognize when we want to detach the child process so that we can
            // unref the process handle and _actually_ detach the child process
            if (option == SubprocessOption::UV_PROCESS_DETACHED)
            {
                mDetach = true;
            }
            flags |= option;
        }
    );
    mProcessOptions.flags = flags;
}

SubprocessHandler::~SubprocessHandler()
{
    char** args = mProcessOptions.args;
    for (size_t i = 0; args[i] != NULL; i++)
    {
        free(args[i]);
    }
    free(args);
}

void SubprocessHandler::RunProcess()
{
    if (mRunning)
    {
        LOG4CXX_ERROR(spLogger, "Process already running");
        return;
    }
    mEOFd = false;
    mExited = false;

    LOG4CXX_DEBUG(spLogger, "Running Process: " << mProcessOptions.args);
    mPipeHandle->Get()->data = this;
    mStdioContainer[1].data.stream = (uv_stream_t*)mPipeHandle->Get();
    mErrPipeHandle->Get()->data = this;
    mStdioContainer[2].data.stream = (uv_stream_t*)mErrPipeHandle->Get();
    mEventLoop->RegisterAndSpawnSubprocess(this);
    uv_read_start(
        (uv_stream_t*)mPipeHandle->Get(), on_alloc, on_read<SubprocessHandler>);
    uv_read_start((uv_stream_t*)mErrPipeHandle->Get(), on_alloc, on_err_read);
    if (mDetach)
    {
        // 'fire-and-forget'; immediately deref the process handle,
        uv_unref((uv_handle_t*)mProcessHandle.Get());
    }
    mRunning = true;
}

void SubprocessHandler::RegisterStdOutCallback(StdOutCallback cb,
                                               bool streaming)
{
    mStdOutCb = cb;
    mStreaming = streaming;
    if (streaming)
    {
        mInternalStdOutCb = std::bind(
            &SubprocessHandler::StreamingRead, this, std::placeholders::_1);
    }
    else
    {
        mInternalStdOutCb = std::bind(
            &SubprocessHandler::BufferedRead, this, std::placeholders::_1);
    }
}

void SubprocessHandler::RegisterStdErrCallback(StdOutCallback cb)
{
    mStdErrCb = cb;
}

void SubprocessHandler::RegisterProcessExitCallback(ProcessExitCallback cb)
{
    mProcessExitCb = cb;
}

bool SubprocessHandler::Running() const
{
    return mRunning;
}

void SubprocessHandler::ReadCalled(ssize_t nread, const uv_buf_t* buf)
{
    if (nread >= 0)
    {
        assert(mInternalStdOutCb);
        SubprocessHandler::Buffer buffer;
        buffer.assign(buf->base, buf->base + nread);
        mInternalStdOutCb(buffer);
    }
    else if (nread == UV_EOF)
    {
        mEOFd = true;
        if (mExited && mEOFd)
        {
            TrueProcessExit();
        }
        else
        {
            uv_read_stop((uv_stream_t*)mPipeHandle->Get());
            uv_read_stop((uv_stream_t*)mErrPipeHandle->Get());
        }
    }
    else if (nread != UV_EOF)
    {
        LOG4CXX_ERROR(spLogger, "Subprocess error: " << uv_strerror(nread));
        std::stringstream buf;
        for (const auto& arg : mCmd)
        {
            buf << arg << " ";
        }
        LOG4CXX_ERROR(spLogger,
                      "Error running process with args: " << buf.str());
        uv_read_stop((uv_stream_t*)mPipeHandle->Get());
        uv_read_stop((uv_stream_t*)mErrPipeHandle->Get());
    }
}

void SubprocessHandler::ErrCalled(ssize_t nread, const uv_buf_t* buf)
{
    if (nread >= 0)
    {
        SubprocessHandler::Buffer buffer;
        buffer.assign(buf->base, buf->base + nread);
        mStdErrBuffer.insert(mStdErrBuffer.end(), buffer.begin(), buffer.end());
    }
}

void SubprocessHandler::StreamingRead(const SubprocessHandler::Buffer& buffer)
{
    CALL_IF_VALID(mStdOutCb, buffer);
}

void SubprocessHandler::BufferedRead(const SubprocessHandler::Buffer& buffer)
{
    mStdOutBuffer.insert(mStdOutBuffer.end(), buffer.begin(), buffer.end());
}

void SubprocessHandler::TrueProcessExit()
{
    if (!mStreaming)
    {
        CALL_IF_VALID(mStdOutCb, mStdOutBuffer);
    }
    mStdOutBuffer.clear();
    mRunning = false;

    CALL_IF_VALID(mStdErrCb, mStdErrBuffer);
    mStdErrBuffer.clear();

    // ref and unref are idempotent, so I don't care if I unref when the handle
    // is already inactive
    // However, if I ref'd the handle to keep the loop going I do need to unref
    // it here so it will
    // stop
    uv_unref((uv_handle_t*)mProcessHandle.Get());
    mProcessHandle.Reset();
    mProcessHandle.Get()->data = this;
    mPipeHandle.reset(new UvHandle<uv_pipe_t>());
    mErrPipeHandle.reset(new UvHandle<uv_pipe_t>());
    CALL_IF_VALID(mProcessExitCb, mExitStatus);
}

void SubprocessHandler::ProcessExited(int status, int /*term_signal*/)
{
    mExited = true;
    mExitStatus = status;
    if (mExited && mEOFd)
    {
        TrueProcessExit();
    }
    else
    {
        // Normally, after process exit the process handle will become inactive
        // and the event loop
        // will stop. However, if we haven't gotten to the EOF, then we need to
        // keep things running
        // until we get our last bit of stdout
        // ref will keep the event loop running in the hopes that the stdout
        // callback will fire and
        // we can truly exit
        uv_ref((uv_handle_t*)mProcessHandle.Get());
    }
}

const std::vector<std::string>& SubprocessHandler::GetCmd() const
{
    return mCmd;
}
}
}
