#pragma once

namespace dctl {
namespace tree {

struct bidirectional_link
{
private:
        using self_type = bidirectional_link;

public:
        // constructors

        bidirectional_link()
        :
                prev_(nullptr),
                next_(nullptr)
        {}

        // modifiers

        friend void set_successor(self_type* lhs, self_type* rhs)
        {
                lhs->next_ = rhs;
                rhs->prev_ = lhs;
        }

        // queries

        self_type* prev() const
        {
                return prev_;
        }

        self_type* next() const
        {
                return next_;
        }

private:
        // representation

        self_type* prev_;
        self_type* next_;
};

}       // namespace tree
}       // namespace dctl
