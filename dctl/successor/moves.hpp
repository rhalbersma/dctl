#pragma once
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {

template<typename, typename>
struct Propagate;

template<typename Position>
struct Propagate<Moves, Position>
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
