#pragma once
#include <type_traits>                  // is_base_of
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT

namespace dctl {

template
<
        typename Derived
>
struct enable_crtp
{
private:
        // typedefs

        typedef enable_crtp Base;

public:
        // casting "down" the inheritance hierarchy
        Derived const* self() const
        {
                BOOST_MPL_ASSERT((std::is_base_of<Base, Derived>));
                return static_cast<Derived const*>(this);
        }

        // write the non-const version in terms of the const version
        // Effective C++ 3rd ed., Item 3 (p. 24-25)
        Derived* self()
        {
                return const_cast<Derived*>(static_cast<Base const*>(this)->self());
        }      

protected:
        // disable deletion of Derived* through Base* 
        // enable deletion of Base* through Derived*
        ~enable_crtp()
        {
                // no-op
        }
};

}       // namespace dctl
