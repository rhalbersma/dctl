#include "Iota.h"

namespace dctl {

Iota::Iota(int start)        
: 
        current_(start)
{
}

int Iota::operator()()
{
        return current_++;
}

}       // namespace dctl
