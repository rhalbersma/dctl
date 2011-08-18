#include <cassert>
#include "Dispatcher.h"
#include "../node/State.h"

namespace dctl {

template<typename Selection, typename Rules> template<bool Color, typename Board>
int Mobility<Selection, Rules>::count(const Position<Board>& p)
{
        return successor::Dispatcher<Selection, Rules, Board>::select(state<Color>(p))->count(p);
}

}       // namespace dctl
