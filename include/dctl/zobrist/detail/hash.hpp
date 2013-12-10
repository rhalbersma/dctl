#pragma once
#include <utility>                              //pair
#include <dctl/zobrist/detail/random.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/restricted.hpp>
#include <dctl/node/side.hpp>
#include <dctl/node/position.hpp>

namespace dctl {

using KingMovesColor = std::pair<KingMoves, bool>;

namespace zobrist {
namespace detail {

// primary template
template<class Index, class Key>
struct hash;

// partial specialization for ab initio hashing of positions
template<class Index, template<class, class> class Position, class Rules, class Board>
struct hash< Index, Position<Rules, Board> >
{
        Index operator()(Position<Rules, Board> const& p) const
        {
                return (
                        Random<Index>::xor_rand(p.pieces(Side::black), Random<Index>::PIECES[Side::black]) ^
                        Random<Index>::xor_rand(p.pieces(Side::white), Random<Index>::PIECES[Side::white]) ^
                        Random<Index>::xor_rand(p.kings()            , Random<Index>::KINGS              ) ^
                        hash<Index, bool      >{}(p.to_move())                                             ^
                        hash<Index, Restricted>{}(p.restricted())
                );
        }
};

// partial specialization for ab initio hashing of moves
template<class Index, template<class, class> class Position, template<class, class> class Move, class Rules, class Board>
struct hash<Index, std::pair<Position<Rules, Board>, Move<Rules, Board>> >
{
        Index operator()(std::pair<Position<Rules, Board>, Move<Rules, Board>> const& m) const
        {
                using T = typename Board::bit_type;
                auto const with_king = active_kings(m.first).test(m.second.from());

                return (
                        Random<Index>::xor_rand(m.second.from()           , Random<Index>::PIECES[ m.first.to_move()]) ^
                        Random<Index>::xor_rand(m.second.dest()           , Random<Index>::PIECES[ m.first.to_move()]) ^
                        Random<Index>::xor_rand(m.second.captured_pieces(), Random<Index>::PIECES[!m.first.to_move()]) ^
                        Random<Index>::xor_rand(m.second.captured_kings() , Random<Index>::KINGS                     ) ^
                        (with_king ? Random<Index>::xor_rand(T{m.second.from(), m.second.dest()}, Random<Index>::KINGS) : Index{}) ^
                        (m.second.promotion() ? Random<Index>::xor_rand(T{m.second.dest()},       Random<Index>::KINGS) : Index{})
                );
        }
};

// partial specialization for ab initio hashing of side to move
template<class Index>
struct hash<Index, bool>
{
        Index operator()(bool color) const
        {
                return Random<Index>::xor_rand(color, Random<Index>::SIDE);
        }
};

// partial specialization for ab initio hashing of restricted consecutive same king moves
template<class Index>
struct hash<Index, KingMovesColor>
{
        Index operator()(KingMovesColor const& restricted) const
        {
                return (
                        Random<Index>::xor_rand(restricted.first.king(),  Random<Index>::RESTRICTED_KING[restricted.second] ) ^
                        Random<Index>::xor_rand(restricted.first.moves(), Random<Index>::RESTRICTED_MOVES[restricted.second])
                );
        }
};

// partial specialization for ab initio hashing of restricted consecutive same king moves
template<class Index>
struct hash<Index, Restricted>
{
        Index operator()(Restricted const& restricted) const
        {
                return (
                        hash<Index, KingMovesColor>{}(std::make_pair(restricted[Side::black], Side::black)) ^
                        hash<Index, KingMovesColor>{}(std::make_pair(restricted[Side::white], Side::white))
                );
        }
};

}       // namespace detail
}       // namespace zobrist
}       // namespace dctl
