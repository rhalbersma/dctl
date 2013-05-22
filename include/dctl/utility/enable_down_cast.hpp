#pragma once
#include <type_traits>                          // is_base_of
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <dctl/preprocessor/cpp11/default.hpp>  // DCTL_PP_IS_DEFAULT

namespace dctl {

template<class Derived>
class enable_down_cast
{
private:
        // typedefs

        typedef enable_down_cast Base;

public:
        Derived const* self() const
        {
                // casting "down" the inheritance hierarchy
                BOOST_MPL_ASSERT(( std::is_base_of<Base, Derived> ));
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
        ~enable_down_cast() DCTL_PP_IS_DEFAULT
};

}       // namespace dctl
