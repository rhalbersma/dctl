#pragma once
#include <array>	// array
#include <cstddef>	// size_t
#include <utility>	// index_sequence, make_index_sequence

namespace dctl {
namespace detail {

template<class Function, std::size_t... Indices>
constexpr auto fill_array_helper(Function f, std::index_sequence<Indices...>)
        -> std::array<decltype(f(std::size_t{})), sizeof...(Indices)>
{
        return {{ f(Indices)... }};
}

}       // namespace detail

template<std::size_t N, class Function>
constexpr auto fill_array(Function f)
{
        return detail::fill_array_helper(f, std::make_index_sequence<N>{});
}

}       // namespace dctl
