#pragma once
#include <algorithm>                    // find
#include "Stack_fwd.hpp"

namespace dctl {

inline
void unique_back(Stack& stack)
{
        if (std::find(stack.begin(), stack.end(), stack.back()) != stack.end() - 1)
                stack.pop_back();
}

}       // namespace dctl
