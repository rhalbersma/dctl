#pragma once
#include <type_traits>                  // is_base_of
#include <boost/static_assert.hpp>      // BOOST_STATIC_ASSERT

namespace dctl {

template
<
        typename Derived
>
struct enable_crtp
{
public:
        Derived const& self() const
        {
                return down_cast(*this);
        }

        Derived& self()
        {
                return down_cast(*this);
        }

protected:
        // disable deletion of Derived* through Base* 
        // enable deletion of Base* through Derived*
        ~enable_crtp()
        {
                // no-op
        }

private:
        // typedefs

        typedef enable_crtp Base;

        // cast a Base& to a Derived& (i.e. "down" the class hierarchy)
        Derived const& down_cast(Base const& other) const
        {
                BOOST_STATIC_ASSERT((std::is_base_of<Base, Derived>::value));
                return static_cast<Derived const&>(other);
        }

        // cast a Base& to a Derived& (i.e. "down" the class hierarchy)
        Derived& down_cast(Base& other)
        {
                // write the non-const version in terms of the const version
                // Effective C++ 3rd ed., Item 3 (p. 24-25)
                return const_cast<Derived&>(down_cast(static_cast<Base const&>(other)));
        }
};

}       // namespace dctl
