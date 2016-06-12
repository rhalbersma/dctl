#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace util {
namespace block_adl {

template<std::size_t>
struct tagged_empty_base
{
        tagged_empty_base() = default;

        template<class... Args>
        constexpr tagged_empty_base(Args&&...) {}
};

}       // namespace block_adl

using block_adl::tagged_empty_base;

}       // namespace util
}       // namespace dctl
