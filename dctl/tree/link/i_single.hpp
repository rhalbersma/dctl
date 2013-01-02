#pragma once
#include <dctl/guarded/default_delete.hpp>      // DCTL_PP_IS_DEFAULT
#include <dctl/utility/enable_down_cast.hpp>    // enable_down_cast

namespace dctl {
namespace tree {
namespace link {

template
<
        typename Node
>
class ISingle
:
        // enable static polymorphism
        public enable_down_cast< Node >
{
private:
        // dependent name now in scope
        using enable_down_cast< Node >::self;

public:
        // typedefs

        typedef Node* node_ptr;

        // modifiers

        void attach(node_ptr other)
        {
                self()->do_attach(other);
        }

        // queries

        node_ptr next() const
        {
                return self()->do_next();
        }

protected:
        // disable deletion of Derived* through Base*
        // enable deletion of Base* through Derived*
        ~ISingle() DCTL_PP_IS_DEFAULT
};

}       // namespace link
}       // namespace tree
}       // namespace dctl
