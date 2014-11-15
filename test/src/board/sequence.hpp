#pragma once
#include <dctl/board/types.hpp>
#include <boost/mpl/vector.hpp>

namespace dctl {
namespace board {

using BoardSequence = boost::mpl::vector
<
        Micro,
        Mini,
        Checkers,
        Roman,
        Spantsireti,
        International,
        Frisian,
        Ktar<10, 11>,
        Ktar<10, 12>,
        Compact1012,
        Compact1210,
        Board<12, 10>,
        Canadian,
        SriLankan,
        Dumm
>;

}       // namespace board
}       // namespace dctl
