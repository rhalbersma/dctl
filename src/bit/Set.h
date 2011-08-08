#pragma once
#include <cstddef>      // std::size_t

namespace dctl {
namespace bit {

template<typename T, typename T, std::size_t N>
class Set
{
public:
        Set<T, N>& flip();
        Set<T, N>& operator&=(const Set<T, N>&);
        Set<T, N>& operator|=(const Set<T, N>&);
        Set<T, N>& operator^=(const Set<T, N>&);
        Set<T, N>& operator<<=(std::size_t);
        Set<T, N>& operator>>=(std::size_t);

        bool operator==(const Set<T, N>&) const;
        bool operator!=(const Set<T, N>&) const;

private
        static const std::size_t BITS_PER_BLOCK = 8 * sizeof(T);
        std::array<T, N> blocks_;
};

template<typename T, std::size_t N> const Set<T, N> operator~(const Set<T, N>&);
template<typename T, std::size_t N> const Set<T, N> operator&(const Set<T, N>&, const Set<T, N>&);
template<typename T, std::size_t N> const Set<T, N> operator|(const Set<T, N>&, const Set<T, N>&);
template<typename T, std::size_t N> const Set<T, N> operator^(const Set<T, N>&, const Set<T, N>&);
template<typename T, std::size_t N> const Set<T, N> operator<<(const Set<T, N>&, std::size_t);
template<typename T, std::size_t N> const Set<T, N> operator>>(const Set<T, N>&, std::size_t);

}       // namespace bit
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Set.hpp"
