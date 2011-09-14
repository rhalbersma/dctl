#pragma once
#include "Token.hpp"

namespace dctl {

namespace board { template<typename, typename> class Board; }

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
