#include <algorithm>    // std::find
#include "Stack.h"
#include "../../Utilities/Bit.h"

namespace tree {
namespace node {

Stack::reference top(Stack& stack)
{
        return stack.back();
}

Stack::const_reference top(const Stack& stack)
{
        return stack.back();
}

// specialization for move generation without duplicate capture checking
bool non_unique_top(const Stack&, Int2Type<false>)
{
        return false;
}

// specialization for move generation without duplicate capture checking
bool non_unique_top(const Stack& stack, Int2Type<true>)
{
        return std::find(stack.begin(), stack.end(), top(stack)) != (stack.end() - 1);
}

void pop(Stack& stack)
{
        return stack.pop_back();
}

}       // namespace node
}       // namespace tree
