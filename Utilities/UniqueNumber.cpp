#include "UniqueNumber.h"

UniqueNumber::UniqueNumber(void)        
: 
        current(0)
{
}

size_t UniqueNumber::operator()(void)
{
        return ++current - 1;
}
