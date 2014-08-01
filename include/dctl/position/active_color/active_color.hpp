#pragma once
#include <dctl/position/active_color/zobrist.hpp>

namespace dctl {

class ActiveColor
{
public:
        // constructors

        ActiveColor() = default;

        explicit ActiveColor(bool color) noexcept
        :
                color_{color}
        {}

        // modifiers

        template<class Move, class  Index>
        void make(Move const&, Index& hash) noexcept
        {
                color_ ^= pass;
                hash ^= random::ActiveColor<>::table_[pass];
        }

        // queries

        /* implicit */ operator bool() const noexcept
        {
                return color_;
        }

private:
        // representation

        enum { pass = true };
        bool color_{};
};

inline
auto init_hash(ActiveColor const& to_move)
{
        return random::ActiveColor<>::table_[to_move];
}

}       // namespace dctl
