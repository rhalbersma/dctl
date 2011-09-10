#include "../rules/Rules.h"

namespace dctl {

template<typename Rules>
bool is_max(PlyCount moves)
{
        return moves == rules::max_same_king_moves<Rules>::value;
}

}       // namespace dctl
