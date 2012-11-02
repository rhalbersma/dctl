#pragma once
#include <dctl/board/board.hpp>
#include <dctl/setup/i_token.hpp>

namespace dctl {

namespace board { template<typename, typename> struct Board; }

namespace setup {

struct bits {};
struct squares {};

template<typename Dimensions, typename Structure>
struct Token< board::Board<Dimensions, Structure> >
{
        typedef squares type;
};

}       // namespace setup
}       // namespace dctl
