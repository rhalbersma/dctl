#pragma once
#include "Setup.h"
#include "../damexchange/Protocol.h"

namespace dctl {
namespace protocol {
namespace setup {

template<>
struct Setup<damexchange::version>
: 
        public Token<'Z', 'W', 'e'> 
{
};

}       // namespace setup
}       // namespace protocol
}       // namespace dctl
