#include "UniqueNumber.h"

namespace dctl {

UniqueNumber::UniqueNumber()        
: 
        current_(0)
{
}

int UniqueNumber::operator()()
{
        return ++current_ - 1;
}

}       // namespace dctl
