#pragma once
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/operators.hpp>                  // totally_ordered
#include <dctl/successor/value_fwd.hpp>         // Value (primary template)
#include <dctl/rules/variants/russian_fwd.hpp>  // Russian

namespace dctl {
namespace successor {

// specialization for Russian draughts
template<>
class Value<rules::Russian>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<rules::Russian> > // < >= > <= == !=
{
public:
        // structors

        Value()
        :
                is_promotion_(false)
        {
                BOOST_ASSERT(invariant());
        }

        explicit Value(bool p)
        :
                is_promotion_(p)
        {
                BOOST_ASSERT(invariant());
        }

        // modifiers

        void toggle_promotion()
        {
                is_promotion_ ^= true;
                BOOST_ASSERT(invariant());
        }

        // queries

        bool is_promotion() const
        {
                return is_promotion_;
        }

        // predicates

        // operator!= provided by boost::totally_ordered
        friend bool operator==(Value const & /* lhs */, Value const& /* rhs */)
        {
                return true;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend bool operator<(Value const & /* lhs */, Value const& /* rhs */)
        {
                return false;
        }

private:
        bool invariant() const
        {
                return true;
        }

        // representation

        bool is_promotion_;
};

}       // namespace successor
}       // namespace dctl
