#pragma once
#include <boost/operators.hpp>          // totally_ordered
#include <cassert>                      // assert
#include <limits>                       // numeric_limits
#include <tuple>                        // get, tie

namespace dctl {
namespace spanish {

class Value
:
        boost::totally_ordered< Value > // < >= > <= == !=
{
public:
        // constructors

        Value() = default;

        explicit Value(std::tuple<int, int> const& t)
        :
                Value{std::get<0>(t), std::get<1>(t)}
        {}

        template<class U>
        explicit Value(U const& u)
        :
                Value{u.num_pieces(), u.num_kings()}
        {}

        Value(int np, int nk)
        :
                num_pieces_{np},
                num_kings_{nk}
        {
                assert(invariant());
        }

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

}       // namespace spanish
}       // namespace dctl
