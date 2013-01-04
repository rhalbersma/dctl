#pragma once

namespace dctl {
namespace tree {

struct backward_link
{
        // structors

        backward_link()
        :
                prev_(nullptr)
        {}

        // modifiers

        void connect(backward_link* other)
        {
                this->prev_ = other;
        }

        // representation

        backward_link* prev_;
};

}       // namespace tree
}       // namespace dctl
