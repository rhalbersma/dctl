#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <dctl/setup/i_token.hpp>
#include <dctl/pdn/version.hpp>

namespace dctl {
namespace setup {

template<>
struct TokenSet<pdn::protocol>
:
        public TokenInterface<'B', 'W', '.'>
{
        BOOST_STATIC_CONSTANT(auto, king  = 'K');
        BOOST_STATIC_CONSTANT(auto, colon = ':');
        BOOST_STATIC_CONSTANT(auto, comma = ',');
        BOOST_STATIC_CONSTANT(auto, quote = '"');
};

}       // namespace setup
}       // namespace dctl
