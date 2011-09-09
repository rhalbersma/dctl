#pragma once
#include <boost/config.hpp>
#include "Token.h"

namespace dctl {
 
namespace pdn { struct protocol; }

namespace setup {

template<>
struct TokenSet<pdn::protocol>
: 
        public TokenSetBase<'B', 'W', '.'>
{
        BOOST_STATIC_CONSTANT(char, KING  = 'K');
        BOOST_STATIC_CONSTANT(char, COLON = ':');
        BOOST_STATIC_CONSTANT(char, COMMA = ',');
        BOOST_STATIC_CONSTANT(char, QUOTE = '"');
};

}       // namespace setup
}       // namespace dctl
