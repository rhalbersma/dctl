#pragma once
#include <utility>      // forward

namespace dctl {
namespace tree {

template
<
        class Link,
        class T
>
struct node
:
        Link
{
        template<class... Args>
        node(Args&&... args)
        :
                Link(),
                value_(std::forward<Args>(args)...)
        {}

        T value_;
};

}       // namespace tree
}       // namespace dctl
