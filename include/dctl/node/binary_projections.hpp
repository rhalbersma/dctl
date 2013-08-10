#pragma once
#include <dctl/node/binary_projections_fwd.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {

template<class Position, class Move>
BitBoard moving_pieces(Position const& p, Move const& m)
{
        return m.pieces(active_color(p));
}

template<class Position, class Move>
BitBoard moving_kings(Position const& p, Move const& m)
{
        return m.kings(active_color(p));
}

template<class Position, class Move>
BitBoard captured_pieces(Position const& p, Move const& m)
{
        return m.pieces(passive_color(p));
}

template<class Position, class Move>
BitBoard captured_kings(Position const& p, Move const& m)
{
        return m.kings(passive_color(p));
}

template<class Position, class Move>
BitBoard from_sq(Position const& p, Move const& m)
{
        return moving_pieces(p, m) & active_pieces(p);
}

template<class Position, class Move>
BitBoard dest_sq(Position const& p, Move const& m)
{
        return moving_pieces(p, m) & not_occupied(p);
}

}       // namespace dctl
