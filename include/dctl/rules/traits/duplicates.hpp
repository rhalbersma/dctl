#pragma once
#include <type_traits>

namespace dctl {
namespace rules {

template<typename>
struct is_remove_duplicates
:
        std::true_type
{};

}       // namespace rules
}       // namespace dctl
