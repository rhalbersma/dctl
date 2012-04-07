#pragma once
#include <algorithm>                    // std::find
#include "Stack_fwd.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"

namespace dctl {

template<typename Rules>
void unique_back(Stack& stack)
{
        // tag dispatching on duplicate capture checking
        aux::unique_back(
                stack,
                Int2Type<rules::is_check_capture_uniqueness<Rules>::value>()
        );
}

namespace aux {

// specialization for move generation without duplicate capture checking
inline
void unique_back(Stack& /* stack */, Int2Type<false>)
{
        // no-op
}

// specialization for move generation with duplicate capture checking
inline
void unique_back(Stack& stack, Int2Type<true>)
{
        if (std::find(stack.begin(), stack.end(), stack.back()) != stack.end() - 1)
                stack.pop_back();
}

}       // namespace aux
}       // namespace dctl
