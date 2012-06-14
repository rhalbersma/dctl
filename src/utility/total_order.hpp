#pragma once

namespace dctl {

template<typename T>
bool is_totally_ordered(const T& left, const T& right)
{
        auto const equivalent = !((left < right) || (right < left));
        auto const equal = left == right;
        return equivalent == equal;
}

}       // namespace dctl
