#pragma once
#include <dctl/core/state/setup/i_token.hpp>
#include <dctl/core/state/ui/pdn/version.hpp>

namespace dctl::core {
namespace setup {

template<>
struct TokenSet<pdn::protocol>
:
        public TokenInterface<'B', 'W', '.'>
{
        static const auto king  = 'K';
        static const auto colon = ':';
        static const auto comma = ',';
        static const auto quote = '"';
};

}       // namespace setup
}       // namespace dctl::core
