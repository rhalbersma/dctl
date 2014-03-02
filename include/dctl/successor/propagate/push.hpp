#pragma once
#include <dctl/successor/propagate_fwd.hpp>
#include <dctl/successor/select/push.hpp>

namespace dctl {
namespace successor {

template<class Position>
struct Propagate<select::push, Position>
{
public:
        // structors

        explicit Propagate(Position const& p)
        :
                not_occupied_(p.not_occupied())
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
        using Set = typename Position::board_type::set_type;
        Set const not_occupied_;
};

}       // namespace successor
}       // namespace dctl
