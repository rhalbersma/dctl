#pragma once
#include <dctl/board/board.hpp>
#include <dctl/setup/i_token.hpp>

namespace dctl {

namespace board { template<class, int, int> struct Board; }

namespace setup {

struct bits {};
struct squares {};

template<class Dimensions, int EdgeColumns, int Orientation>
struct Token< board::Board<Dimensions, EdgeColumns, Orientation> >
{
        using type = squares;
};

}       // namespace setup
}       // namespace dctl
