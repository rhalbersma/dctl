#pragma once
#include "Token.h"

namespace dctl {
 
namespace dxp { struct protocol; }

namespace setup {

// forward declaration of the primary template
template<typename> struct Setup;

template<>
struct Setup<dxp::protocol>
: 
        public Token<'Z', 'W', 'e'> 
{
};

}       // namespace setup
}       // namespace dctl
