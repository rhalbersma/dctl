#include "UniqueNumber.h"

UniqueNumber::UniqueNumber(void)        
: 
        current_(0)
{
}

int UniqueNumber::operator()(void)
{
        return ++current_ - 1;
}
