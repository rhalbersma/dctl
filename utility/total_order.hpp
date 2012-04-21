#pragma once

namespace dctl {

template<typename T>
bool is_totally_ordered(const T& left, const T& right)
{
        const auto equivalent = !((left < right) || (right < left));
        const auto equal = left == right;
        return equivalent == equal;
}

}       // namespace dctl
