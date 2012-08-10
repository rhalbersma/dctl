#pragma once
#include <cstdint>                      // uint64_t

namespace dctl {

template
<
        typename T,
        typename Link
>
struct Node
:
        public Link
{
        // structors

        Node(T const& v)
        :
                Link(),
                value_(v)
        {}

        // representation

        T value_;
};

}       // namespace dctl
