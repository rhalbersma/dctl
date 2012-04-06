#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT

namespace dctl {
namespace capture {

template<typename Implementation>
struct OrderInterface
{
public:
        // delegate to a derived class
        bool operator<(const OrderInterface<Implementation>& other) const
        {
                return self().less(down_cast(other));
        }

        // delegate to the operator<
        bool operator>(const OrderInterface<Implementation>& other) const
        {
                return other < *this;
        }

        // delegate to the operator<
        bool operator>=(const OrderInterface<Implementation>& other) const
        {
                return !(*this < other);
        }

        // delegate to the operator<
        bool operator<=(const OrderInterface<Implementation>& other) const
        {
                return !(other < *this);
        }

        // delegate to a derived class
        bool operator==(const OrderInterface<Implementation>& other) const
        {
                const auto equality = self().equal(down_cast(other));
                BOOST_ASSERT(equality == equivalence(other));
                return equality;
        }

        // delegate to the operator==
        bool operator!=(const OrderInterface<Implementation>& other) const
        {
                return !(*this == other);
        }

private:
        // cast a base reference to a derived reference (i.e. "down" the class hierarchy)
        const Implementation& down_cast(const OrderInterface<Implementation>& other) const
        {
                return static_cast<const Implementation&>(other);
        }

        // cast the this pointer to a derived reference
        const Implementation& self() const
        {
                return down_cast(*this);
        }

        // a strict weak order induces a total order over indifference equivalence classes
        bool equivalence(const OrderInterface<Implementation>& other) const
        {
                return !((*this < other) || (other < *this));
        }
};

}       // namespace capture
}       // namespace dctl
