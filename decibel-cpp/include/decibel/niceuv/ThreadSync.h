#pragma once

#include <memory>
#include <functional>

#include "uv.h"

#include "boost/noncopyable.hpp"

namespace decibel
{
namespace niceuv
{

template<typename T, int (*Initializer)(T*), void (*Destructor)(T*)>
class ThreadSynchronizationPrimitive: private boost::noncopyable
{
public:
    ThreadSynchronizationPrimitive()
        : mValid(false)
    {
        if (Initializer(&mHandle) >= 0)
        {
            mValid = true;
        }
    }

    virtual ~ThreadSynchronizationPrimitive()
    {
        if (Valid())
        {
            Destructor(&mHandle);
        }
    }

    bool Valid() const
    {
        return mValid;
    }

    void EnsureValid() const {
        if (!Valid()) {
            throw std::runtime_error("primitive not valid");
        }
    }

    T* Get() {
        if (Valid())
        {
            return &mHandle;
        }

        return nullptr;
    }

protected:
    T mHandle;
    bool mValid;
};

class UvMutex: public ThreadSynchronizationPrimitive<uv_mutex_t, uv_mutex_init, uv_mutex_destroy>
{
public:
    void Lock() {
        EnsureValid();

        uv_mutex_lock(&mHandle);
    }

    void Unlock() {
        EnsureValid();

        uv_mutex_unlock(&mHandle);
    }

    int TryLock() {
        EnsureValid();

        return uv_mutex_trylock(&mHandle);
    }
};

class UvMutexGuard
{
public:
    explicit UvMutexGuard(UvMutex *guarded_mutex)
        : mGuardedMutex(guarded_mutex)
    {
        mGuardedMutex->Lock();
    }

    ~UvMutexGuard()
    {
        mGuardedMutex->Unlock();
    }

private:
    UvMutex *mGuardedMutex;
};

class UvCondVar: public ThreadSynchronizationPrimitive<uv_cond_t, uv_cond_init, uv_cond_destroy>
{
public:
    void Signal()
    {
        EnsureValid();

        uv_cond_signal(&mHandle);
    }

    void Broadcast()
    {
        EnsureValid();

        uv_cond_broadcast(&mHandle);
    }

    void Wait(UvMutex *m)
    {
        EnsureValid();

        uv_cond_wait(&mHandle, m->Get());
    }

    int TimedWait(UvMutex *m, uint64_t timeout)
    {
        EnsureValid();

        return uv_cond_timedwait(&mHandle, m->Get(), timeout);
    }
};

}  // namespace niceuv
}  // namespace decibel
