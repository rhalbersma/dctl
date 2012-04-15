#pragma once
#include <type_traits>
#include <boost/static_assert.hpp>

namespace dctl {

template
<
        typename Derived
>
struct enable_crtp
{
public:
        const Derived& self() const
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
        const Derived& down_cast(const Base& other) const
        {
                BOOST_STATIC_ASSERT((std::is_base_of<Base, Derived>::value));
                return static_cast<const Derived&>(other);
        }

        // cast a Base& to a Derived& (i.e. "down" the class hierarchy)
        Derived& down_cast(Base& other)
        {
                // write the non-const version in terms of the const version
                // Effective C++ 3rd ed., Item 3 (p. 24-25)
                return const_cast<Derived&>(down_cast(static_cast<const Base&>(other)));
        }
};

}       // namespace dctl
