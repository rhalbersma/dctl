#pragma once
#include <functional>                   // function
#include <dctl/bit/bit_fwd.hpp>         // first::clear, is_element, singlet
#include <dctl/utility/int.hpp>         // num_bits

namespace dctl {
namespace bit {
namespace loop {
namespace detail {

template<typename T>
struct count
{
        std::size_t operator()(T b)
        {
                // Kernighan & Ritchie, The C programming language, 2nd Ed.
                // https://chessprogramming.wikispaces.com/Population+Count
                std::size_t n = 0;
                for (; b; first::clear(b))
                        ++n;
                return n;
        }
};

template<typename T>
struct index
{
        int operator()(T b)
        {
                for (std::size_t i = 0; i < num_bits<T>::value; ++i)
                        if (is_element(singlet<T>(i), b))
                                return i;
                return 0;
        }
};

// TODO: partial specializations for bit arrays

}       // namespace detail

template<typename T>
std::size_t count(T b)
{
        return detail::count<T>()(b);
}

template<typename T>
int index(T b)
{
        return detail::index<T>()(b);
}

}       // namespace loop
}       // namespace bit
}       // namespace dctl
