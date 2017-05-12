#include "decibel/niceuv/WorkCancellable.h"

namespace decibel
{
namespace niceuv
{

WorkCancellable::WorkCancellable(std::shared_ptr<uv_work_t> work)
    : mWork(std::move(work))
{
}

void WorkCancellable::Cancel()
{
    if (uv_cancel(reinterpret_cast<uv_req_t*>(mWork.get())) < 0)
    {
        throw std::runtime_error("cancel failed");
    }
}

}  // namespace niceuv
}  // namespace decibel
