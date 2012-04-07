#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT

namespace dctl {

template<typename Impl>
struct TotalOrderInterface
{
public:
        // delegate to a derived class
        bool operator<(const TotalOrderInterface<Impl>& other) const
        {
                return self().less(down_cast(other));
        }

        // delegate to the operator<
        bool operator>(const TotalOrderInterface<Impl>& other) const
        {
                return other < *this;
        }

        // delegate to the operator<
        bool operator>=(const TotalOrderInterface<Impl>& other) const
        {
                return !(*this < other);
        }

        // delegate to the operator<
        bool operator<=(const TotalOrderInterface<Impl>& other) const
        {
                return !(other < *this);
        }

        // delegate to a derived class rather than to the operator<
        //---------------------------------------------------------------------
        // RATIONALE: equality is more efficiently implemented than equivalence
        // e.g. for extended types such as N-tuples <a1 ... aN> and <b1 ... bN>
        // 1 call to std::equal takes at most N comparisons
        // 2 calls to std::lexicographical_compare take at most 4 N comparisons
        //---------------------------------------------------------------------
        bool operator==(const TotalOrderInterface<Impl>& other) const
        {
                // for a total order, equivalence MUST correspond to equality
                // "trust efficiency, but verify correctness"
                const auto equality = self().equal(down_cast(other));
                BOOST_ASSERT(equality == equivalence(other));
                return equality;
        }

        // delegate to the operator==
        bool operator!=(const TotalOrderInterface<Impl>& other) const
        {
                return !(*this == other);
        }

private:
        // cast a base reference to a derived reference (i.e. "down" the class hierarchy)
        const Impl& down_cast(const TotalOrderInterface<Impl>& other) const
        {
                return static_cast<const Impl&>(other);
        }

        // cast the this pointer to a derived reference
        const Impl& self() const
        {
                return down_cast(*this);
        }

        //  a and b are equivalent if and only if neither a < b nor b < a
        bool equivalence(const TotalOrderInterface<Impl>& other) const
        {
                return !((*this < other) || (other < *this));
        }
};

}       // namespace dctl
