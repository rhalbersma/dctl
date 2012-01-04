#pragma once
#include "TokenInterface.hpp"

namespace dctl {
 
namespace dxp { struct protocol; }

namespace setup {

template<>
struct TokenSet<dxp::protocol>
: 
        public TokenInterface<'Z', 'W', 'e'> 
{
};

}       // namespace setup
}       // namespace dctl
