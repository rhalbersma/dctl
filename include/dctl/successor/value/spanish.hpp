#pragma once
#include <dctl/successor/value_fwd.hpp> // Value (primary template)
#include <dctl/rules/spanish_fwd.hpp>   // Spanish
#include <boost/operators.hpp>          // totally_ordered
#include <cassert>                      // assert
#include <limits>                       // numeric_limits
#include <tuple>                        // get, tie

namespace dctl {
namespace successor {

// specialization for Spanish draughts
template<>
class Value<rules::Spanish>
:
        boost::totally_ordered< Value<rules::Spanish> > // < >= > <= == !=
{
public:
        // constructors

        Value() = default;

        explicit Value(std::tuple<int, int> const& t)
        :
                Value{std::get<0>(t), std::get<1>(t)}
        {}

        Value(int pawns, int kings)
        :
                num_pieces_{pawns + kings},
                num_kings_{kings}
        {
                assert(invariant());
        }

        template<class U>
        explicit Value(U const& u)
        :
                num_pieces_{u.num_pieces()},
                num_kings_{u.num_kings()}
        {}

        // predicates

        // operator!= provided by boost::totally_ordered
        friend /* constexpr */ auto
        operator==(Value const& lhs, Value const& rhs) noexcept
        {
                // delegate to std::tuple::operator==
                return
                        std::tie(lhs.num_pieces_, lhs.num_kings_) ==
                        std::tie(rhs.num_pieces_, rhs.num_kings_)
                ;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend /* constexpr */ auto
        operator<(Value const& lhs, Value const& rhs) noexcept
        {
                // delegate to std::tuple::operator<
                return
                        std::tie(lhs.num_pieces_, lhs.num_kings_) <
                        std::tie(rhs.num_pieces_, rhs.num_kings_)
                ;
        }

private:
        // contracts

        constexpr bool invariant() const noexcept
        {
                return 0 <= num_kings_ && num_kings_ <= num_pieces_;
        }

        // representation

        int num_pieces_{};
        int num_kings_{};
};

}       // namespace successor
}       // namespace dctl
