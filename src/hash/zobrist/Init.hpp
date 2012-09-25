#pragma once
#include <functional>                   // function
#include "Random.hpp"
#include "../../node/Material.hpp"
#include "../../node/Move.hpp"
#include "../../node/Restricted.hpp"
#include "../../node/Side.hpp"

namespace dctl {
namespace hash {
namespace zobrist {

// primary template
template<typename Index, typename Key>
struct Init;

// partial specialization for ab initio hashing of material
template<typename Index>
struct Init<Index, Material>
:
        std::function<Index(Material const&)>
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
:
        std::function<Index(Move const&)>
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
:
        std::function<Index(bool)>
{
        Index operator()(bool color) const
        {
                return Random<Index>::xor_rand(color, Random<Index>::SIDE);
        }
};

// partial specialization for ab initio hashing of restricted consecutive same king moves
template<typename Index>
struct Init<Index, Restricted>
:
        std::function<Index(Restricted const&)>
{
        Index operator()(Restricted const& restricted) const
        {
                return (
                        Init<Index, KingMoves>()(restricted[Side::black], Side::black) ^
                        Init<Index, KingMoves>()(restricted[Side::white], Side::white)
                );
        }
};

// partial specialization for ab initio hashing of restricted consecutive same king moves
template<typename Index>
struct Init<Index, KingMoves>
:
        std::function<Index(KingMoves const&, bool)>
{
        Index operator()(KingMoves const& restricted, bool color) const
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
