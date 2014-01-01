#pragma once
#include <dctl/move/move.hpp>           // Move
#include <dctl/zobrist/accumulate.hpp>  // accumulate
#include <dctl/zobrist/material.hpp>    // Material

namespace dctl {

template<class Rules, class Board>
uint64_t zobrist_hash(Move<Rules, Board> const& m, bool to_move)
{
        using Zobrist = random::Material<Board::set_type::N>;

        auto index = uint64_t{0};
        index ^= Zobrist::pieces[to_move][static_cast<std::size_t>(m.from())];
        index ^= Zobrist::pieces[to_move][static_cast<std::size_t>(m.dest())];
        if (m.is_with_king()) {
                index ^= Zobrist::kings[static_cast<std::size_t>(m.from())];
                index ^= Zobrist::kings[static_cast<std::size_t>(m.dest())];
        } else if (m.is_promotion()) {
                index ^= Zobrist::kings[static_cast<std::size_t>(m.dest())];
        }
        if (m.is_jump()) {
                index ^= zobrist::accumulate(m.captured_pieces(), Zobrist::pieces[!to_move]);
                index ^= zobrist::accumulate(m.captured_kings() , Zobrist::kings           );
        }
        return index;
}

}       // namespace dctl
