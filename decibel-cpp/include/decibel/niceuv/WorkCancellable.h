#pragma once

#include <memory>

#include <uv.h>

#include "decibel/niceuv/UvHandle.h"

namespace decibel
{
namespace niceuv
{

class EventLoop;

class WorkCancellable
{
friend EventLoop;

public:
    void Cancel();

private:
    explicit WorkCancellable(std::shared_ptr<uv_work_t> work);

    std::shared_ptr<uv_work_t> mWork;
};

} // namespace niceuv
} // namespace decibel
