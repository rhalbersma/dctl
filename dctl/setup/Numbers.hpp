#pragma once
#include <dctl/setup/TokenInterface.hpp>

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
