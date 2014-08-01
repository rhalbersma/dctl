#pragma once

namespace dctl {
namespace tree {

struct forward_link
{
private:
        using self_type = forward_link;

public:
        // constructors

        forward_link()
        :
                next_(nullptr)
        {}

        // modifiers

        friend void set_successor(self_type* lhs, self_type* rhs)
        {
                lhs->next_ = rhs;
        }

        // queries

        self_type* next() const
        {
                return next_;
        }

private:
        // representation

        self_type* next_;
};

}       // namespace tree
}       // namespace dctl
