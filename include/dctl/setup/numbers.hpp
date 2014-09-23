#pragma once
#include <dctl/board/board.hpp>
#include <dctl/setup/i_token.hpp>

namespace dctl {

namespace board { template<int, int, bool, bool> class Board; }

namespace setup {

struct bits {};
struct squares {};

template<int W, int H, bool I, bool IsOrthogonalCaptures>
struct Token< board::Board<W, H, I, IsOrthogonalCaptures>>
{
        using type = squares;
};

}       // namespace setup
}       // namespace dctl
