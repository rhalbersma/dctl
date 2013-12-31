#pragma once
#include <cstdint>                      // uint64_t
#include <dctl/utility/make_array.hpp>  // array, make_array
#include <dctl/zobrist/generate.hpp>    // zero_generate

namespace dctl {
namespace zobrist {

struct Side
{
        using index_type = uint64_t;

        static std::array<index_type, 2> const to_move;
};

std::array<typename Side::index_type, 2> const
Side::to_move = make_array<2>(zero_generate);

}       // namespace zobrist
}       // namespace dctl
