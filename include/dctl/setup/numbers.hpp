#pragma once
#include <dctl/board/board.hpp>
#include <dctl/setup/i_token.hpp>

namespace dctl {

namespace board { template<class, class> struct Board; }

namespace setup {

struct bits {};
struct squares {};

template<class Dimensions, class Structure>
struct Token< board::Board<Dimensions, Structure> >
{
        using type = squares;
};

}       // namespace setup
}       // namespace dctl
