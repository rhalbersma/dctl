#include "UniqueNumber.h"

UniqueNumber::UniqueNumber()        
: 
        current_(0)
{
}

int UniqueNumber::operator()()
{
        return ++current_ - 1;
}
