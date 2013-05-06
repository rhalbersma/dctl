#pragma once
//#include <utility>      // forward

namespace dctl {
namespace tree {

template
<
        typename Link,
        typename T
>
struct node
:
        Link
{
        template<typename... Args>
        node(Args&&... args)
        :
                Link(),
                value_(std::forward<Args>(args)...)
        {}

        T value_;
};

}       // namespace tree
}       // namespace dctl
