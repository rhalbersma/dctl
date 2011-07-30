#pragma once
#include "Token.h"

namespace dctl {
 
namespace pdn { struct protocol; }

namespace setup {

// forward declaration of the primary template
template<typename> struct Setup;

template<>
struct Setup<pdn::protocol>
: 
        public Token<'B', 'W', '.'>
{
        static const char KING  = 'K';
        static const char COLON = ':';
        static const char COMMA = ',';
        static const char QUOTE = '"';
};

}       // namespace setup
}       // namespace dctl
