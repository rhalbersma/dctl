#pragma once
#include "../../utility/enable_crtp.hpp"        // enable_crtp

namespace dctl {
namespace tree {
namespace link {

template
<
        typename Impl,
>
struct LinkInterface
:
        // enable static polymorphism
        private enable_crtp< Impl >
{
public:
        // modifiers

        void attach(Impl* other)
        {
                self()->attach(other);
        }

        void detach()
        {
                self()->detach();
        }

protected:
        // disable deletion of Derived* through Base*
        // enable deletion of Base* through Derived*
        ~LinkInterface()
        {
                // no-op
        }
};

}       // namespace link
}       // namespace tree
}       // namespace dctl
