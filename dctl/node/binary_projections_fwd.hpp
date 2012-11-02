#pragma once
#include <dctl/utility/int.hpp>

namespace dctl {

template<typename Position, typename Move>
BitBoard moving_pieces(Position const&, Move const&);

template<typename Position, typename Move>
BitBoard moving_kings(Position const&, Move const&);

template<typename Position, typename Move>
BitBoard captured_pieces(Position const&, Move const&);

template<typename Position, typename Move>
BitBoard captured_kings(Position const&, Move const&);

template<typename Position, typename Move>
BitBoard from_sq(Position const&, Move const&);

template<typename Position, typename Move>
BitBoard dest_sq(Position const&, Move const&);

}       // namespace dctl
