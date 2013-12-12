#pragma once
#include <boost/preprocessor/config/limits.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {

struct KingMoves
{
public:
        // structors

        constexpr KingMoves() = default;

        // modifiers

        constexpr void init(int dest) noexcept
        {
                king_ = dest;
                moves_ = 1;
        }

        constexpr void increment(int dest) noexcept
        {
                king_ = dest;
                ++moves_;
        }

        constexpr void reset() noexcept
        {
                king_ = UNDEFINED;
                moves_ = 0;
        }

        // queries

        constexpr auto king() const noexcept
        {
                return king_;
        }

        constexpr auto moves() const noexcept
        {
                return moves_;
        }

private:
        // implementation

        enum { UNDEFINED = BOOST_PP_LIMIT_REPEAT };

        // representation

        int king_{UNDEFINED};
        int moves_{};
};

using Restricted = KingMoves[2];

// predicates
template<class Rules>
constexpr auto is_max(int moves) noexcept
{
        return (moves == rules::max_same_king_moves<Rules>::value);
}

}       // namespace dctl
