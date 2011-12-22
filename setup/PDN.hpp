#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Token.hpp"

namespace dctl {
 
namespace pdn { struct protocol; }

namespace setup {

template<>
struct TokenSet<pdn::protocol>
: 
        public TokenSetBase<'B', 'W', '.'>
{
        BOOST_STATIC_CONSTANT(auto, king  = 'K');
        BOOST_STATIC_CONSTANT(auto, colon = ':');
        BOOST_STATIC_CONSTANT(auto, comma = ',');
        BOOST_STATIC_CONSTANT(auto, quote = '"');
};

}       // namespace setup
}       // namespace dctl