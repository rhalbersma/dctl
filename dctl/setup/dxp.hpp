#pragma once
#include <dctl/setup/i_token.hpp>

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
