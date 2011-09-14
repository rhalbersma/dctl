#include "Random.h"
#include "../../node/Position.h"
#include "../../node/Move.h"
#include "../../node/Material.h"
#include "../../node/Restricted.hpp"
#include "../../node/Side.hpp"

namespace dctl {
namespace hash {
namespace zobrist {

// partial specialization for ab initio hashing of positions
template<typename Board, typename Index>
struct Init<Position<Board>, Index>
: 
        public std::unary_function<Position<Board>, Index>
{
        Index operator()(const Position<Board>& p) const
        {
                return (
                        Init<Material  , Index>()(p.material())     ^
                        Init<bool      , Index>()(p.active_color()) ^
                        Init<Restricted, Index>()(p.restricted())
                );
        }
};

// partial specialization for ab initio hashing of moves
template<typename Index>
struct Init<Move, Index>
: 
        public std::unary_function<Move, Index>
{
        Index operator()(const Move& m) const
        {
                return (
                        Init<Material, Index>()(m.material()) ^
                        Init<bool    , Index>()(m.to_move())
                );
        }
};

// partial specialization for ab initio hashing of material
template<typename Index>
struct Init<Material, Index>
: 
        public std::unary_function<Material, Index>
{
        Index operator()(const Material& p) const
        {
                return (
        	        Random<Index>::xor_rand(p.pieces(Side::black), Random<Index>::PIECES[Side::black]) ^
                        Random<Index>::xor_rand(p.pieces(Side::white), Random<Index>::PIECES[Side::white]) ^
                        Random<Index>::xor_rand(p.kings()            , Random<Index>::KINGS              )
                );
        }
};

// partial specialization for ab initio hashing of side to move
template<typename Index>
struct Init<bool, Index>
: 
        public std::unary_function<bool, Index>
{
        Index operator()(bool to_move) const
        {
                return Random<Index>::xor_rand(to_move, Random<Index>::SIDE);
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
