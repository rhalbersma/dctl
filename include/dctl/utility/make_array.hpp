#pragma once
#include <array>
#include <cstddef>
#include <dctl/cpp14/integer_sequence.hpp>

namespace dctl {
namespace detail {

template<class Function, std::size_t... Indices>
constexpr auto make_array_helper(Function f, cpp14::index_sequence<Indices...>)
-> std::array<decltype(f(std::size_t{})), sizeof...(Indices)>
{
        return {{ f(Indices)... }};
}

}       // namespace detail

template<std::size_t N, class Function>
constexpr auto make_array(Function f)
-> std::array<decltype(f(std::size_t{})), N>
{
        return detail::make_array_helper(f, cpp14::make_index_sequence<N>{});
}

}       // namespace dctl
