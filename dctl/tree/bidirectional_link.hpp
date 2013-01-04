#pragma once

namespace dctl {
namespace tree {

struct bidirectional_link
{
        bidirectional_link()
        :
                next_(nullptr),
                prev_(nullptr)
        {}

        void connect(bidirectional_link* other)
        {
                this->next_ = other;
                other->prev_ = this;
        }

        bidirectional_link* next_;
        bidirectional_link* prev_;
};

}       // namespace tree
}       // namespace dctl
