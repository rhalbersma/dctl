#include <algorithm>
#include "Stack.h"
#include "../../Utilities/Bit.h"

tree::move::Stack::reference tree::move::top(Stack* stack)
{
        return stack->back();
}

tree::move::Stack::const_reference tree::move::top(const Stack& stack)
{
        return stack.back();
}

// specialization for move generation without duplicate capture checking
bool tree::move::non_unique_top(const Stack&, Int2Type<false>)
{
        return false;
}

// specialization for move generation without duplicate capture checking
bool tree::move::non_unique_top(const Stack& stack, Int2Type<true>)
{
        return std::find(stack.begin(), stack.end(), top(stack)) != (stack.end() - 1);
}


void tree::move::pop(Stack* stack)
{
        return stack->pop_back();
}
