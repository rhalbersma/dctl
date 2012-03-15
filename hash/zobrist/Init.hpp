#pragma once
#include <functional>                   // unary_function
#include "Random.hpp"
#include "../../node/Move.hpp"
#include "../../node/Restricted.hpp"
#include "../../node/Side.hpp"

namespace dctl {
namespace hash {
namespace zobrist {

// primary template
template<typename Key, typename Index>
struct Init;

// partial specialization for ab initio hashing of moves
template<typename Index>
struct Init<Move, Index>
:
        public std::unary_function<Move, Index>
{
        Index operator()(const Move& m) const
        {
                return (
                        Random<Index>::xor_rand(m.pieces(Side::black), Random<Index>::PIECES[Side::black]) ^
                        Random<Index>::xor_rand(m.pieces(Side::white), Random<Index>::PIECES[Side::white]) ^
                        Random<Index>::xor_rand(m.kings()            , Random<Index>::KINGS              )
                );
        }
};

// partial specialization for ab initio hashing of side to move
template<typename Index>
struct Init<bool, Index>
:
        public std::unary_function<bool, Index>
{
        Index operator()(bool color) const
        {
                return Random<Index>::xor_rand(color, Random<Index>::SIDE);
        }
};

// partial specialization for ab initio hashing of restricted consecutive same king moves
template<typename Index>
struct Init<Restricted, Index>
:
        public std::unary_function<Restricted, Index>
{
        Index operator()(const Restricted& restricted) const
        {
                return (
                        Init<KingMoves, Index>()(restricted[Side::black], Side::black) ^
                        Init<KingMoves, Index>()(restricted[Side::white], Side::white)
                );
        }
};

// partial specialization for ab initio hashing of restricted consecutive same king moves
template<typename Index>
struct Init<KingMoves, Index>
:
        public std::binary_function<KingMoves, bool, Index>
{
        Index operator()(const KingMoves& restricted, bool color) const
        {
                return (
                        Random<Index>::xor_rand(restricted.king(),  Random<Index>::RESTRICTED_KING[color] ) ^
                        Random<Index>::xor_rand(restricted.moves(), Random<Index>::RESTRICTED_MOVES[color])
                );
        }
};

}       // namespace zobrist
}       // namespace hash
}       // namespace dctl
