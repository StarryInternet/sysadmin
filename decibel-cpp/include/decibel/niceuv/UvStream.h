
#pragma once

#include <uv.h>

namespace decibel
{
namespace niceuv
{
#define CALL_IF_VALID(func, arg)                                               \
    if (func)                                                                  \
    {                                                                          \
        func(arg);                                                             \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        LOG4CXX_DEBUG(log4cxx::Logger::getLogger("UvStream"),                  \
                      "Callback " << #func << " not bound");                   \
    }

template <typename T>
void on_read(uv_stream_t* handler, ssize_t nread, const uv_buf_t* buf)
{
    T* reader = reinterpret_cast<T*>(handler->data);
    reader->ReadCalled(nread, buf);
    free(buf->base);
}

void on_alloc(uv_handle_t*, size_t sug_size, uv_buf_t* buf);

void on_write(uv_write_t* req, int);
}
}
