#pragma once
#include "../../utility/enable_down_cast.hpp"
#include <iterator>

namespace dctl {
namespace tree {
namespace link {

template
<
        template<typename> class Node, 
        typename T
>
struct ISingle
:
        // enable static polymorphism
        private enable_down_cast< Node<T> >
{
private:
        // dependent name now in scope
        using enable_down_cast< Node<T> >::self;

public:
        // typedefs

        typedef Node<T>* node_ptr;

        // modifiers

        void attach(node_ptr other)
        {
                self()->do_attach(other);
        }

        void detach()
        {
                self()->do_detach();
        }

        // queries

        node_ptr next() const
        {
                return self()->do_next();
        }

protected:
        // disable deletion of Derived* through Base*
        // enable deletion of Base* through Derived*
        ~ISingle()
        {
                // no-op
        }
};

}       // namespace link
}       // namespace tree
}       // namespace dctl
