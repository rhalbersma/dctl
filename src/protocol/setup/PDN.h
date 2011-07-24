#pragma once
#include "Setup.h"
#include "../pdn/Protocol.h"

namespace dctl {
namespace protocol {
namespace setup {

template<>
struct Setup<pdn::version>
: 
        public Token<'B', 'W', '.'>
{
        static const char KING  = 'K';
        static const char COLON = ':';
        static const char COMMA = ',';
        static const char QUOTE = '\"';
};

}       // namespace setup
}       // namespace protocol
}       // namespace dctl
