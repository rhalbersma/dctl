#pragma once
#include "Token.hpp"

namespace dctl {
 
namespace dxp { struct protocol; }

namespace setup {

template<>
struct TokenSet<dxp::protocol>
: 
        public TokenSetBase<'Z', 'W', 'e'> 
{
};

}       // namespace setup
}       // namespace dctl
