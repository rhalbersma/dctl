#pragma once
#include <cassert>                              // assert
#include <boost/operators.hpp>                  // totally_ordered
#include <dctl/successor/value_fwd.hpp>         // Value (primary template)
#include <dctl/rules/variants/czech_fwd.hpp>    // Czech

namespace dctl {
namespace successor {

// specialization for Czech draughts
template<>
class Value<rules::Czech>
:
        boost::totally_ordered< Value<rules::Czech> > // < >= > <= == !=
{
public:
        // structors

        Value() = default;

        explicit Value(bool w)
        :
                is_with_king_{w}
        {}

        template<class Move>
        explicit Value(Move const& m)
        :
                Value{m.is_with_king()}
        {}

        // modifiers

        void toggle_with_king()
        {
                is_with_king_ ^= true;
        }

        void increment(bool) { ++n_; }
        void decrement(bool) { --n_; }
        int size() const { return n_; }

        // queries

        auto is_with_king() const
        {
                return is_with_king_;
        }

        // predicates

        // operator!= provided by boost::totally_ordered
        friend auto operator==(Value const& lhs, Value const& rhs)
        {
                return lhs.is_with_king_ == rhs.is_with_king_;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend auto operator<(Value const& lhs, Value const& rhs)
        {
                return lhs.is_with_king_ < rhs.is_with_king_;
        }

private:
        // representation
        int n_{};
        bool is_with_king_{};
};

}       // namespace successor
}       // namespace dctl
