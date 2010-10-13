#pragma once
#include "IntegerTypes.h"

class UniqueNumber
{
public:
        UniqueNumber(void);
        size_t operator()(void);

private:
        size_t current;
};
