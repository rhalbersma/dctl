#pragma once
#include <dctl/setup/i_token.hpp>
#include <dctl/ui/dxp/version.hpp>

namespace dctl {
namespace setup {

template<>
struct TokenSet<dxp::protocol>
:
        public TokenInterface<'Z', 'W', 'e'>
{
};

}       // namespace setup
}       // namespace dctl
