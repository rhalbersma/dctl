#pragma once
#include <iterator>                     // begin, end
#include <dctl/node/stack.hpp>

namespace dctl {
namespace search {

/*
inline Variation make_variation(int move, Variation const& continuation)
{
        Variation line(1, move);
        line.insert(std::end(line), std::begin(continuation), std::end(continuation));
        return line;
}
*/

inline void update_variation(Variation& line, int move, Variation const& continuation)
{
        line.clear();
        line.push_back(move);
        line.insert(std::end(line), std::begin(continuation), std::end(continuation));
}

}       // namespace search
}       // namespace dctl
