#pragma once
#include <cstdint>                      // uint64_t
#include <dctl/utility/make_array.hpp>  // array, make_array
#include <dctl/zobrist/generate.hpp>    // generate

namespace dctl {
namespace zobrist {

template<int N>
struct Material
{
        using index_type = uint64_t;

        static std::array<index_type, N> const pieces[];
        static std::array<index_type, N> const kings;
};

template<int N>
std::array<typename Material<N>::index_type, N> const
Material<N>::pieces[] =
{
        make_array<N>(generate),
        make_array<N>(generate)
};

template<int N>
std::array<typename Material<N>::index_type, N> const
Material<N>::kings = make_array<N>(generate);

}       // namespace zobrist
}       // namespace dctl
