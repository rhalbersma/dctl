#pragma once
#include <cassert>                              // assert
#include <boost/operators.hpp>                  // totally_ordered
#include <dctl/successor/value_fwd.hpp>         // Value (primary template)
#include <dctl/rules/variants/russian_fwd.hpp>  // Russian

namespace dctl {
namespace successor {

// specialization for Russian draughts
template<>
class Value<rules::Russian>
:
        boost::totally_ordered< Value<rules::Russian> > // < >= > <= == !=
{
public:
        // structors

        Value() = default;

        explicit Value(bool p)
        :
                is_promotion_{p}
        {
                assert(invariant());
        }

        // modifiers

        void toggle_promotion()
        {
                is_promotion_ ^= true;
                assert(invariant());
        }

        // queries

        bool is_promotion() const
        {
                return is_promotion_;
        }

        // predicates

        // operator!= provided by boost::totally_ordered
        friend bool operator==(Value const& /* lhs */, Value const& /* rhs */)
        {
                return true;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend bool operator<(Value const& /* lhs */, Value const& /* rhs */)
        {
                return false;
        }

private:
        bool invariant() const
        {
                return true;
        }

        // representation

        bool is_promotion_ {};
};

}       // namespace successor
}       // namespace dctl
