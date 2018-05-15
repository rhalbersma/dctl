#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>  // conditional, is_empty

namespace dctl {
namespace util {
namespace block_adl {

template<class Tag>
struct tagged_empty
{
        tagged_empty() = default;

        template<class... Args>
        constexpr explicit tagged_empty(Args&&...) noexcept {}
};

template<bool Condition, class Base>
struct conditional_base
:
        std::conditional_t<Condition, Base, tagged_empty<Base>>
{
        static_assert(std::is_empty_v<tagged_empty<Base>>);
};

}       // namespace block_adl

using block_adl::conditional_base;

}       // namespace util
}       // namespace dctl
