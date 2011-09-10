#include "State.h"
#include "../node/Material.h"
#include "../node/Side.h"

namespace dctl {
namespace successor {

template<typename Selection, typename Rules, typename Board>
typename Dispatcher<Selection, Rules, Board>::StatePointer Dispatcher<Selection, Rules, Board>::select(int state)
{
        return instance()[state];
}

template<typename Selection, typename Rules, typename Board>
typename Dispatcher<Selection, Rules, Board>::StateArray& Dispatcher<Selection, Rules, Board>::instance()
{
        static const State<Side::BLACK, Material::NONE, Selection, Rules, Board> BLACK_NONE_;
        static const State<Side::BLACK, Material::PAWN, Selection, Rules, Board> BLACK_PAWN_;
        static const State<Side::BLACK, Material::KING, Selection, Rules, Board> BLACK_KING_;
        static const State<Side::BLACK, Material::BOTH, Selection, Rules, Board> BLACK_BOTH_;
        static const State<Side::WHITE, Material::NONE, Selection, Rules, Board> WHITE_NONE_;
        static const State<Side::WHITE, Material::PAWN, Selection, Rules, Board> WHITE_PAWN_;
        static const State<Side::WHITE, Material::KING, Selection, Rules, Board> WHITE_KING_;
        static const State<Side::WHITE, Material::BOTH, Selection, Rules, Board> WHITE_BOTH_;

        // Meyers Singleton, see Modern C++ Design p.117
        static const StateArray singleton_ = {
                &BLACK_NONE_, &BLACK_PAWN_, &BLACK_KING_, &BLACK_BOTH_, 
                &WHITE_NONE_, &WHITE_PAWN_, &WHITE_KING_, &WHITE_BOTH_
        };
        return singleton_;
}

}       // namespace successor
}       // namespace dctl
