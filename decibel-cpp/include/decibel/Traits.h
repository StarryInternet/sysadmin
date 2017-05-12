
#pragma once

namespace decibel
{
template <typename F, typename... A>
using ResultOf = decltype(std::declval<F>()(std::declval<A>()...));

} // decibel
