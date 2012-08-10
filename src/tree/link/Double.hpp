#pragma once
#include "LinkInterface.hpp"

namespace dctl {
namespace tree {
namespace link {

struct Double
:
        public LinkInterface<Double>
{
        // structors

        Double()
        :
                prev_(nullptr),
                next_(nullptr)
        {}

        // representation

        Double* prev_;
        Double* next_;

private:
        // implementation

        void attach(Double* other)
        {
                next_ = other;
                next_->prev_ = this;
        }

        void detach()
        {
                next_ = next_->prev_ = nullptr;
        }
};

}       // namespace link
}       // namespace tree
}       // namespace dctl
