#pragma once
#include <dctl/successor/propagate_fwd.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/bit/bitboard.hpp>                // BitBoard

namespace dctl {
namespace successor {

template<class Position>
struct Propagate<select::moves, Position>
{
public:
        // structors

        explicit Propagate(Position const& p)
        :
                not_occupied_(not_occupied(p))
        {}

        // queries

        BitBoard path() const
        {
                return not_occupied_;
        }

private:
        // representation

        BitBoard not_occupied_;
};

}       // namespace successor
}       // namespace dctl
