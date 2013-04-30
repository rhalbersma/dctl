#pragma once
#include <dctl/node/binary_projections_fwd.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {

template<typename Position, typename Move>
BitBoard moving_pieces(Position const& p, Move const& m)
{
        return m.pieces(active_color(p));
}

template<typename Position, typename Move>
BitBoard moving_kings(Position const& p, Move const& m)
{
        return m.kings(active_color(p));
}

template<typename Position, typename Move>
BitBoard captured_pieces(Position const& p, Move const& m)
{
        return m.pieces(passive_color(p));
}

template<typename Position, typename Move>
BitBoard captured_kings(Position const& p, Move const& m)
{
        return m.kings(passive_color(p));
}

template<typename Position, typename Move>
BitBoard from_sq(Position const& p, Move const& m)
{
        return moving_pieces(p, m) & active_pieces(p);
}

template<typename Position, typename Move>
BitBoard dest_sq(Position const& p, Move const& m)
{
        return moving_pieces(p, m) & not_occupied(p);
}

}       // namespace dctl
