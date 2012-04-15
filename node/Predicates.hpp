#pragma once
#include "Position.hpp"
#include "Move.hpp"
#include "../bit/Bit.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename Board>
BitBoard from_sq(const Position<Board>& p, const Move& m)
{
        return moving_pieces(p, m) & active_pieces(p);
}

template<typename Board>
BitBoard dest_sq(const Position<Board>& p, const Move& m)
{
        return moving_pieces(p, m) & not_occupied(p);
}

template<typename Board>
BitBoard moving_pieces(const Position<Board>& p, const Move& m)
{
        return m.pieces(p.active_color());
}

template<typename Board>
BitBoard moving_kings(const Position<Board>& p, const Move& m)
{
        return m.kings(p.active_color());
}

template<typename Board>
BitBoard captured_pieces(const Position<Board>& p, const Move& m)
{
        return m.pieces(p.passive_color());
}

template<typename Board>
BitBoard captured_kings(const Position<Board>& p, const Move& m)
{
        return m.kings(p.passive_color());
}

template<typename Board>
bool is_connected(const Position<Board>& p, const Move& m1, const Move& m2)
{
        return false;
}

template<typename Board>
bool is_reversible(const Position<Board>& p, const Move& m)
{
        return is_with_king(p, m) && !is_capture(p, m);
}

template<typename Board>
bool is_promotion(const Position<Board>& p, const Move& m)
{
        return bit::is_single(moving_kings(p, m));
}

template<typename Board>
bool is_with_king(const Position<Board>& p, const Move& m)
{
        return !bit::is_zero(moving_kings(p, m) & active_kings(p));
}

template<typename Board>
bool is_capture(const Position<Board>& p, const Move& m)
{
        return !bit::is_zero(captured_pieces(p, m));
}

template<typename Rules, typename Board>
bool is_pseudo_legal(const Position<Board>& p, const Move& m)
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
                        is_intersecting_capture<Rules>(p, m)
                )
        );
}

template<typename Rules, typename Board>
bool is_intersecting_capture(const Position<Board>& p, const Move& m)
{
        // tag dispatching on capture removal
        return aux::is_intersecting_capture(
                p, m,
                Int2Type<rules::jump_removal<Rules>::value>()
        );
}

namespace aux {

// partial specialization for apres-fini capture removal
template<typename Board>
bool is_intersecting_capture(const Position<Board>& /* p */, const Move& /* m */, Int2Type<rules::remove_af>)
{
        return false;
}

// partial specialization for en-passant capture removal
template<typename Board>
bool is_intersecting_capture(const Position<Board>& p, const Move& m, Int2Type<rules::remove_ep>)
{
        // for intersecting captures, a man-capturing king can appear as a captured king
        return bit::is_single(moving_kings(p, m) & captured_kings(p, m) & passive_pawns(p));
}

}       // namespace aux
}       // namespace dctl
