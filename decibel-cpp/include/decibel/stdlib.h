#pragma once

#include <memory>

#if !defined __cpp_lib_make_unique && !defined __APPLE__

namespace std
{
template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}

#endif
