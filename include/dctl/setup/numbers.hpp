#pragma once
#include <dctl/board/board.hpp>
#include <dctl/setup/i_token.hpp>

namespace dctl {

namespace board { template<class, bool> struct Board; }

namespace setup {

struct bits {};
struct squares {};

template<class Dimensions, bool IsOrthogonalCaptures>
struct Token< board::Board<Dimensions, IsOrthogonalCaptures> >
{
        using type = squares;
};

}       // namespace setup
}       // namespace dctl
