#include "State.h"
#include "../node/Material.h"
#include "../node/Side.h"

namespace dctl {
namespace successor {

template<typename Rules, typename Board>
typename Dispatcher<Rules, Board>::StatePointer Dispatcher<Rules, Board>::select(int state)
{
        return instance()[state];
}

template<typename Rules, typename Board>
typename Dispatcher<Rules, Board>::StateArray& Dispatcher<Rules, Board>::instance()
{
        static const State<Side::BLACK, Material::NONE, Rules, Board> BLACK_NONE_;
        static const State<Side::BLACK, Material::PAWN, Rules, Board> BLACK_PAWN_;
        static const State<Side::BLACK, Material::KING, Rules, Board> BLACK_KING_;
        static const State<Side::BLACK, Material::BOTH, Rules, Board> BLACK_BOTH_;
        static const State<Side::WHITE, Material::NONE, Rules, Board> WHITE_NONE_;
        static const State<Side::WHITE, Material::PAWN, Rules, Board> WHITE_PAWN_;
        static const State<Side::WHITE, Material::KING, Rules, Board> WHITE_KING_;
        static const State<Side::WHITE, Material::BOTH, Rules, Board> WHITE_BOTH_;

        // Meyers Singleton, see Modern C++ Design p.117
        static const StateArray singleton_ = {
                &BLACK_NONE_, &BLACK_PAWN_, &BLACK_KING_, &BLACK_BOTH_, 
                &WHITE_NONE_, &WHITE_PAWN_, &WHITE_KING_, &WHITE_BOTH_
        };
        return singleton_;
}

}       // namespace successor
}       // namespace dctl
