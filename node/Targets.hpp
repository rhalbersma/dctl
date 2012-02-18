#pragma once
#include "Position.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<bool Color, typename Rules, typename Board>
static BitBoard targets(const Position<Board>& p)
{
        // tag dispatching on whether men can capture kings
        return targets_dispatch<Color>(
                p, Int2Type<rules::is_men_capture_kings<Rules>::value>()
        );
}

// partial specialization for men that cannot capture kings
template<bool Color, typename Board>
static BitBoard targets_dispatch(
        const Position<Board>& p, Int2Type<false>
)
{
        return p.men(!Color);
}

// partial specialization for men that can capture kings
template<bool Color, typename Board>
static BitBoard targets_dispatch(
        const Position<Board>& p, Int2Type<true>
)
{
        return p.pieces(!Color);
}

}       // namespace dctl
