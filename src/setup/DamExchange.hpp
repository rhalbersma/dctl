#include "Setup.h"

namespace dctl {
namespace setup {

template<>
struct Setup<protocol::damexchange::version>
: 
        public Token<'Z', 'W', 'e'> 
{
};

}       // namespace setup
}       // namespace dctl
