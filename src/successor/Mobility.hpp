#pragma once
#include "Dispatcher.hpp"
#include "Result.hpp"

namespace dctl {
namespace mobility {

template<bool Color, typename Selection, template<typename, typename> class Position, typename Rules, typename Board>
int count(Position<Rules, Board> const& p)
{
        typedef successor::Dispatcher<Selection, successor::enumeration, Position, Rules, Board> Delegate;
        return (*Delegate::select(state<Color>(p)))(p);
}

}       // namespace mobility
}       // namespace dctl
