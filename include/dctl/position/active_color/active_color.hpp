#pragma once
#include <dctl/position/active_color/zobrist.hpp>

namespace dctl {

class ActiveColor
{
public:
        // structors

        ActiveColor() = default;

        explicit ActiveColor(bool active_color) noexcept
        :
                active_color_{active_color}
        {}

        // modifiers

        template<class Move, class  Index>
        void make(Move const&, Index& hash) noexcept
        {
                active_color_ ^= pass;
                hash ^= random::ActiveColor<>::table_[pass];
        }

        // queries

        /* implicit */ operator bool() const noexcept
        {
                return active_color_;
        }

private:
        // representation

        enum { pass = true };
        bool active_color_{};
};

inline
auto init_hash(ActiveColor const& to_move)
{
        return random::ActiveColor<>::table_[to_move];
}

}       // namespace dctl
