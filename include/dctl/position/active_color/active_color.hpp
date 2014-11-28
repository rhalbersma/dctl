#pragma once
#include <dctl/position/active_color/zobrist.hpp>

namespace dctl {

class ActiveColor
{
        bool color_{};
        enum { pass = true };

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
                hash ^= zobrist::ActiveColor<>::color[pass];
        }

        // observers

        /* implicit */ operator bool() const noexcept
        {
                return color_;
        }
};

template<class TabulationHash>
auto hash_xor_accumulate(TabulationHash const& h, ActiveColor const& to_move)
{
        return h.color[to_move];
}

}       // namespace dctl
