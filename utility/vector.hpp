#pragma once
#include <vector>                       // vector

namespace dctl {

template<typename T>
std::vector<T> operator+(const std::vector<T>& lhs, const std::vector<T>& rhs)
{
        auto tmp = lhs;
        tmp.insert(tmp.end(), rhs.begin(), rhs.end());
        return tmp;
}

template<typename T>
std::vector<T> operator+(const std::vector<T>& lhs, std::vector<T>&& rhs)
{
        rhs.insert(rhs.begin(), lhs.begin(), lhs.end());
        return std::move(rhs);
}

template<typename T>
std::vector<T> operator+(std::vector<T>&& lhs, const std::vector<T>& rhs)
{
        lhs.insert(lhs.end(), rhs.begin(), rhs.end());
        return std::move(lhs);
}

template<typename T>
std::vector<T> operator+(std::vector<T>&& lhs, std::vector<T>&& rhs)
{
        lhs.insert(lhs.end(), rhs.begin(), rhs.end());
        return std::move(lhs);
}

}       // namespace dctl
