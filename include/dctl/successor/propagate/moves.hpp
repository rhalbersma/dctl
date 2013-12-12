#pragma once
#include <dctl/successor/propagate_fwd.hpp>
#include <dctl/successor/select/moves.hpp>

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

        auto path() const
        {
                return not_occupied_;
        }

        auto path(int sq) const
        {
                return path().test(sq);
        }

private:
        // representation
        using T = typename Position::board_type::set_type;
        T const not_occupied_;
};

}       // namespace successor
}       // namespace dctl
