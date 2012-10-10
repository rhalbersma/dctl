#pragma once
#include "Token.h"

namespace dctl {
 
namespace pdn { struct protocol; }

namespace setup {

template<>
struct TokenSet<pdn::protocol>
: 
        public TokenSetBase<'B', 'W', '.'>
{
        static const char KING  = 'K';
        static const char COLON = ':';
        static const char COMMA = ',';
        static const char QUOTE = '"';
};

}       // namespace setup
}       // namespace dctl
