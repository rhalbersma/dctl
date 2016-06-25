#pragma once
#include <dctl/setup/i_token.hpp>
#include <dctl/ui/pdn/version.hpp>

namespace dctl {
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

}       // namespace maskup
}       // namespace dctl
