#pragma once

namespace dctl {

template<typename T>
bool is_totally_ordered(T const& left, T const& right)
{
        auto const equivalence = !((left < right) || (right < left));
        auto const equality = left == right;
        return equivalence == equality;
}

}       // namespace dctl
