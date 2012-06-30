#pragma once
#include "LinkInterface.hpp"

namespace dctl {
namespace tree {
namespace link {

struct Single
:
        public LinkInterface<Single>
{
        // structors

        Single()
        :
                next_(nullptr)
        {}

        // representation

        Single* next_;

private:
        // implementation

        void attach(Single* other)
        {
                next_ = other;
        }

        void detach()
        {
                next_ = nullptr;
        }
};

}       // namespace link
}       // namespace tree
}       // namespace dctl
