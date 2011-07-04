#include <algorithm>    // std::find
#include "Stack.h"
#include "../../Utilities/Bit.h"

tree::node::Stack::reference tree::node::top(Stack& stack)
{
        return stack.back();
}

tree::node::Stack::const_reference tree::node::top(const Stack& stack)
{
        return stack.back();
}

// specialization for move generation without duplicate capture checking
bool tree::node::non_unique_top(const Stack&, Int2Type<false>)
{
        return false;
}

// specialization for move generation without duplicate capture checking
bool tree::node::non_unique_top(const Stack& stack, Int2Type<true>)
{
        return std::find(stack.begin(), stack.end(), top(stack)) != (stack.end() - 1);
}

void tree::node::pop(Stack& stack)
{
        return stack.pop_back();
}
