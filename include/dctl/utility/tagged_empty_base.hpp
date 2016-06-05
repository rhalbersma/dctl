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

// deriving from tagged_empty_base will not make util an associated namespace
// this prevents ADL from finding overloads for unrestricted templates
using block_adl::tagged_empty_base;

}       // namespace util
}       // namespace dctl
