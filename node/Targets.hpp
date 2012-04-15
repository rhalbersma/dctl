#pragma once
#include "Position.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<bool Color, typename Rules, typename Board>
static BitBoard targets(const Position<Board>& p)
{
        // tag dispatching on whether pawns can capture kings
        return aux::targets<Color>(p, Int2Type<rules::is_pawns_jump_kings<Rules>::value>());
}

namespace aux {

// partial specialization for pawns that cannot capture kings
template<bool Color, typename Board>
static BitBoard targets(const Position<Board>& p, Int2Type<false>)
{
        return p.pawns(!Color);
}

// partial specialization for pawns that can capture kings
template<bool Color, typename Board>
static BitBoard targets(const Position<Board>& p, Int2Type<true>)
{
        return p.pieces(!Color);
}

}       // namespace aux
}       // namespace dctl
