#pragma once
#include <cassert>                              // assert
#include <limits>                               // numeric_limits
#include <boost/operators.hpp>                  // totally_ordered
#include <dctl/successor/value_fwd.hpp>         // Value (primary template)
#include <dctl/rules/international_fwd.hpp>     // International

namespace dctl {
namespace successor {

// partial specialization for International draughts
template<>
class Value<rules::International>
:
        boost::totally_ordered< Value<rules::International> >   // !=, >=, >, <=
{
public:
        // structors

        constexpr Value() = default;

        explicit constexpr Value(int n) noexcept
        :
                num_pieces_{n}
        {}

        template<class U>
        explicit constexpr Value(U const& u) noexcept
        :
                num_pieces_{u.num_pieces()}
        {}

        // predicates

        // operator!= provided by boost::totally_ordered
        friend constexpr auto
        operator==(Value const& lhs, Value const& rhs) noexcept
        {
                return lhs.num_pieces_ == rhs.num_pieces_;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend constexpr auto
        operator<(Value const& lhs, Value const& rhs) noexcept
        {
                return lhs.num_pieces_ < rhs.num_pieces_;
        }

private:
        // representation

        int num_pieces_{};
};

}       // namespace successor
}       // namespace dctl
