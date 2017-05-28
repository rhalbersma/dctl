#pragma once
#include <dctl/core/board.hpp>
#include <boost/mpl/vector.hpp>

namespace dctl {
namespace board {

using BoardSequence = boost::mpl::vector
<
        micro,
        mini,
        checkers,
        italian,
        spantsiretti,
        international,
        frisian,
        ktar<10, 11>,
        ktar<10, 12>,
        rectangular<12, 10>,
        canadian,
        srilankan,
        dumm
>;

}       // namespace board
}       // namespace dctl
