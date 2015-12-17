#pragma once
#include <dctl/utility/static_vector.hpp>       // Variation

namespace dctl {
namespace search {

inline void update_variation(Variation& line, int move, Variation const& continuation)
{
        line.clear();
        line.push_back(move);
        line.insert(line.begin(), continuation.begin(), continuation.end());
}

}       // namespace search
}       // namespace dctl
