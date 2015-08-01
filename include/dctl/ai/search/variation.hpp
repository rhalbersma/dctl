#pragma once
#include <iterator>                     // begin, end
#include <dctl/utility/stack_vector.hpp>

namespace dctl {
namespace search {

inline void update_variation(Variation& line, int move, Variation const& continuation)
{
        line.clear();
        line.push_back(move);
        line.insert(end(line), begin(continuation), end(continuation));
}

}       // namespace search
}       // namespace dctl
