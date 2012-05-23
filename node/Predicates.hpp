#pragma once
#include <boost/mpl/identity.hpp>       // identity
#include "../bit/Bit.hpp"
#include "../rules/Enum.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

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

template<typename Position, typename Move>
BitBoard moving_pieces(Position const& p, Move const& m)
{
        return m.pieces(p.active_color());
}

template<typename Position, typename Move>
BitBoard moving_kings(Position const& p, Move const& m)
{
        return m.kings(p.active_color());
}

template<typename Position, typename Move>
BitBoard captured_pieces(Position const& p, Move const& m)
{
        return m.pieces(p.passive_color());
}

template<typename Position, typename Move>
BitBoard captured_kings(Position const& p, Move const& m)
{
        return m.kings(p.passive_color());
}

template<typename Position, typename Move>
bool is_connected(Position const& p, Move const& m1, Move const& m2)
{
        return false;
}

template<typename Position, typename Move>
bool is_reversible(Position const& p, Move const& m)
{
        return is_with_king(p, m) && !is_capture(p, m);
}

template<typename Position, typename Move>
bool is_promotion(Position const& p, Move const& m)
{
        return bit::is_single(moving_kings(p, m));
}

template<typename Position, typename Move>
bool is_with_king(Position const& p, Move const& m)
{
        return !bit::is_zero(moving_kings(p, m) & active_kings(p));
}

template<typename Position, typename Move>
bool is_capture(Position const& p, Move const& m)
{
        return !bit::is_zero(captured_pieces(p, m));
}

template<typename Position, typename Move>
bool is_pseudo_legal(Position const& p, Move const& m)
{
        return (
                // cannot move multiple pieces
                !(bit::is_multiple(from_sq(p, m)) || bit::is_multiple(dest_sq(p, m))) &&

                // only capture existing pieces
                bit::is_subset_of(captured_pieces(p, m), passive_pieces(p)) &&
                (
                        // only capture existing kings
                        bit::is_subset_of(captured_kings(p, m), passive_kings(p)) ||

                        // EXCEPTION: for intersecting captures, a man-capturing king can appear as a captured king
                        is_intersecting_capture(p, m)
                )
        );
}

template<typename Rules, typename Board, template<typename, typename> class Position, typename Move>
bool is_intersecting_capture(Position<Rules, Board> const& p, Move const& m)
{
        // tag dispatching on capture removal
        return detail::is_intersecting_capture(
                p, m,
                boost::mpl::identity<typename Rules::jump_removal>()
        );
}

namespace detail {

// partial specialization for apres-fini capture removal
template<typename Position, typename Move>
bool is_intersecting_capture(
        Position const& /* p */, Move const& /* m */, 
        boost::mpl::identity<rules::removal::apres_fini>
)
{
        return false;
}

// partial specialization for en-passant capture removal
template<typename Position, typename Move>
bool is_intersecting_capture(
        Position const& p, Move const& m, 
        boost::mpl::identity<rules::removal::en_passant>
)
{
        // for intersecting captures, a man-capturing king can appear as a captured king
        return bit::is_single(moving_kings(p, m) & captured_kings(p, m) & passive_pawns(p));
}

}       // namespace detail
}       // namespace dctl
