
#pragma once

#include <uv.h>

namespace decibel
{
namespace niceuv
{
enum
{
    UV_CLOSING = 0x01, /* uv_close() called but not finished. */
    UV_CLOSED = 0x02,  /* close(2) finished. */
};

template <class T>
class UvHandle
{
public:
    UvHandle() : mHandle(new T), mValid(false)
    {
    }

    ~UvHandle()
    {
        if (Valid() &&
            !(((uv_handle_t*)mHandle)->flags & (UV_CLOSING | UV_CLOSED)))
        {
            uv_close((uv_handle_t*)mHandle, UvHandle<T>::closer);
        }
        else
        {
            delete mHandle;
        }
    }

    T* Get()
    {
        return mHandle;
    }

    void MakeValid()
    {
        mValid = true;
    }

    bool Valid() const
    {
        return mValid;
    }

private:
    static void closer(uv_handle_t* handle)
    {
        delete reinterpret_cast<T*>(handle);
    }

private:
    T* mHandle;
    bool mValid;
};
}
}
