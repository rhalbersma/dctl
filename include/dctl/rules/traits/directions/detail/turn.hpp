#pragma once
#include <dctl/rules/types.hpp>

namespace dctl {
namespace rules {
namespace directions {
namespace detail {

// primary template
template<class T>
struct turn
:
        T
{};

// specialization for orthogonal directions
template<>
struct turn<orth>
:
        diag
{};

}       // namespace detail
}       // namespace directions
}       // namespace rules
}       // namespace turn
