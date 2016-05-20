#pragma once
#include <dctl/board/types.hpp>
#include <boost/mpl/vector.hpp>

namespace dctl {
namespace board {

using BoardSequence = boost::mpl::vector
<
        micro,
        mini,
        checkers,
        roman,
        spantsireti,
        international,
        frisian,
        ktar<10, 11>,
        ktar<10, 12>,
        compact_10_12,
        compact_12_10,
        canadian,
        srilankan,
        dumm
>;

}       // namespace board
}       // namespace dctl
