#pragma once
#include <utility>                      //pair
#include <dctl/hash/zobrist/random.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/restricted.hpp>
#include <dctl/node/side.hpp>

namespace dctl {

typedef std::pair<KingMoves, bool> KingMovesColor;

namespace hash {
namespace zobrist {

// primary template
template<typename Index, typename Key>
struct Init;

// partial specialization for ab initio hashing of material
template<typename Index>
struct Init<Index, Material>
{
        Index operator()(Material const& m) const
        {
                return (
                        Random<Index>::xor_rand(m.pieces(Side::black), Random<Index>::PIECES[Side::black]) ^
                        Random<Index>::xor_rand(m.pieces(Side::white), Random<Index>::PIECES[Side::white]) ^
                        Random<Index>::xor_rand(m.kings()            , Random<Index>::KINGS              )
                );
        }
};

// partial specialization for ab initio hashing of moves
template<typename Index>
struct Init<Index, Move>
{
        Index operator()(Move const& m) const
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
struct Init<Index, bool>
{
        Index operator()(bool color) const
        {
                return Random<Index>::xor_rand(color, Random<Index>::SIDE);
        }
};

// partial specialization for ab initio hashing of restricted consecutive same king moves
template<typename Index>
struct Init<Index, KingMovesColor>
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
template<typename Index>
struct Init<Index, Restricted>
{
        Index operator()(Restricted const& restricted) const
        {
                return (
                        Init<Index, KingMovesColor>()(std::make_pair(restricted[Side::black], Side::black)) ^
                        Init<Index, KingMovesColor>()(std::make_pair(restricted[Side::white], Side::white))
                );
        }
};

template<typename Index, typename T>
Index hash(T const& t)
{
        return Init<Index, T>()(t);
}

}       // namespace zobrist
}       // namespace hash
}       // namespace dctl
