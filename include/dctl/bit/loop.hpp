#pragma once
#include <dctl/bit/bit_fwd.hpp>         // first::clear, is_element, singlet
#include <dctl/utility/int.hpp>         // num_bits

namespace dctl {
namespace bit {
namespace loop {
namespace detail {

template<typename T>
struct count
{
        int operator()(T b)
        {
                // Kernighan & Ritchie, The C programming language, 2nd Ed.
                // https://chessprogramming.wikispaces.com/Population+Count
                auto n = 0;
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
                for (auto i = 0; i < num_bits<T>::value; ++i)
                        if (is_element(singlet<T>(i), b))
                                return i;
                return 0;
        }
};

// TODO: partial specializations for bit arrays

}       // namespace detail

template<typename T>
int count(T b)
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
