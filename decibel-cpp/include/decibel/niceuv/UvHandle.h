
#pragma once

#include <uv.h>

namespace decibel
{
namespace niceuv
{

template <class T>
class UvHandle final
{
public:
    UvHandle() : mHandle(new T), mValid(false)
    {
    }

    UvHandle(const UvHandle&) = delete;

    ~UvHandle()
    {
        CloseOrFreeHandle();
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

    void Reset() {
        CloseOrFreeHandle();

        mValid = false;
        mHandle = new T;
    }

private:
    static void closer(uv_handle_t* handle)
    {
        delete reinterpret_cast<T*>(handle);
    }

    void CloseOrFreeHandle() {
        auto generic_handle = reinterpret_cast<uv_handle_t*>(mHandle);

        if (Valid() && uv_is_closing(generic_handle) == 0)
        {
            uv_close(generic_handle, UvHandle<T>::closer);
        }
        else
        {
            delete mHandle;
        }
    }

private:
    T* mHandle;
    bool mValid;
};
}
}
