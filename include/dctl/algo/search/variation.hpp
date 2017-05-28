#pragma once
#include <dctl/util/static_vector.hpp>       // Variation

namespace dctl {
namespace aima {
namespace search {

inline void update_variation(Variation& line, int move, Variation const& continuation)
{
        line.clear();
        line.push_back(move);
        line.insert(line.begin(), continuation.cbegin(), continuation.cend());
}

}       // namespace search
}       // namespace aima
}       // namespace dctl
