#pragma once
#include <type_traits>

namespace dctl {
namespace util {

template<class T>
constexpr auto is_trivial_special_members_v =
        std::is_trivially_destructible<T>{} &&
        std::is_trivially_default_constructible<T>{} &&
        std::is_trivially_copy_constructible<T>{} &&
        std::is_trivially_copy_assignable<T>{} &&
        std::is_trivially_move_constructible<T>{} &&
        std::is_trivially_move_assignable<T>{}
;

}       // namespace util
}       // namespace dctl
