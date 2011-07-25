#include "Setup.h"

namespace dctl {
namespace setup {

template<>
struct Setup<protocol::pdn::version>
: 
        public Token<'B', 'W', '.'>
{
        static const char KING  = 'K';
        static const char COLON = ':';
        static const char COMMA = ',';
        static const char QUOTE = '\"';
};

}       // namespace setup
}       // namespace dctl
