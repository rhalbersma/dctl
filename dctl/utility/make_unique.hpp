#pragma once
#include <memory>                       // unique_ptr
#include <type_traits>
#include <utility>

namespace dctl {
namespace detail {

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(std::false_type, Args&&... args)
{
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(std::true_type, Args&&... args)
{
        static_assert(std::extent<T>::value == 0, "make_unique<T[N]>() is forbidden, please use make_unique<T[]>()");
        typedef typename std::remove_extent<T>::type U;
        return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
}

}       // namespace detail

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
        return detail::make_unique<T>(std::is_array<T>(), std::forward<Args>(args)...);
}

}       // namespace dctl
