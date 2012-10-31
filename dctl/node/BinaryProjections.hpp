#pragma once
#include <dctl/node/BinaryProjections_fwd.hpp>
#include <dctl/node/UnaryProjections.hpp>
#include <dctl/utility/IntegerTypes.hpp>

namespace dctl {

template<typename Position, typename Move>
BitBoard moving_pieces(Position const& p, Move const& m)
{
        return (m.pieces(p.active_color()));
}

template<typename Position, typename Move>
BitBoard moving_kings(Position const& p, Move const& m)
{
        return (m.kings(p.active_color()));
}

template<typename Position, typename Move>
BitBoard captured_pieces(Position const& p, Move const& m)
{
        return (m.pieces(p.passive_color()));
}

template<typename Position, typename Move>
BitBoard captured_kings(Position const& p, Move const& m)
{
        return (m.kings(p.passive_color()));
}

template<typename Position, typename Move>
BitBoard from_sq(Position const& p, Move const& m)
{
        return (moving_pieces(p, m) & active_pieces(p));
}

template<typename Position, typename Move>
BitBoard dest_sq(Position const& p, Move const& m)
{
        return (moving_pieces(p, m) & not_occupied(p));
}

}       // namespace dctl
