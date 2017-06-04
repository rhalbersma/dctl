#pragma once
#include <dctl/core/state/setup/i_token.hpp>
#include <dctl/core/state/ui/dxp/version.hpp>

namespace dctl {
namespace core {
namespace setup {

template<>
struct TokenSet<dxp::protocol>
:
        public TokenInterface<'Z', 'W', 'e'>
{
};

}       // namespace setup
}       // namespace core
}       // namespace dctl
