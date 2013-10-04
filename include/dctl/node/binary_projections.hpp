#pragma once
#include <dctl/node/binary_projections_fwd.hpp>
#include <dctl/node/unary_projections.hpp>

namespace dctl {

template<class Position, class Move>
auto moving_pieces(Position const& p, Move const& m)
{
        return m.pieces(active_color(p));
}

template<class Position, class Move>
auto moving_kings(Position const& p, Move const& m)
{
        return m.kings(active_color(p));
}

template<class Position, class Move>
auto captured_pieces(Position const& p, Move const& m)
{
        return m.pieces(passive_color(p));
}

template<class Position, class Move>
auto captured_kings(Position const& p, Move const& m)
{
        return m.kings(passive_color(p));
}

template<class Position, class Move>
auto from_sq(Position const& p, Move const& m)
{
        return moving_pieces(p, m) & active_pieces(p);
}

template<class Position, class Move>
auto dest_sq(Position const& p, Move const& m)
{
        return moving_pieces(p, m) & not_occupied(p);
}

}       // namespace dctl
