#pragma once
#include <type_traits>                          // is_base_of

namespace dctl {

template<class Derived>
class enable_down_cast
{
private:
        // types

        using Base = enable_down_cast;

public:
        Derived const* self() const
        {
                // casting "down" the inheritance hierarchy
                static_assert(std::is_base_of<Base, Derived>::value, "");
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
        ~enable_down_cast() = default;
};

}       // namespace dctl
