#pragma once
#include <dctl/successor/propagate_fwd.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {

template<typename Position>
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