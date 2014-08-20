#pragma once
#include <dctl/successor/propagate_fwd.hpp>
#include <dctl/successor/select/push.hpp>
#include <dctl/type_traits.hpp>         // board_type_t, rules_type_t

namespace dctl {
namespace successor {

template<class Position>
class Propagate<select::push, Position>
{
public:
        // constructors

        explicit Propagate(Position const& p)
        :
                not_occupied_(p.not_occupied())
        {}

        // observers

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
        using Set = set_type_t<Position>;
        Set const not_occupied_;
};

}       // namespace successor
}       // namespace dctl
