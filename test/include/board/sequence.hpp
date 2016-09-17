#pragma once
#include <dctl/board.hpp>
#include <boost/mpl/vector.hpp>

namespace dctl {
namespace board {

using BoardSequence = boost::mpl::vector
<
        Micro,
        Mini,
        Checkers,
        Italian,
        Spantsiretti,
        International,
        Frisian,
        Ktar<10, 11>,
        Ktar<10, 12>,
        Rectangular<12, 10>,
        Canadian,
        SriLankan,
        Dumm
>;

}       // namespace board
}       // namespace dctl
