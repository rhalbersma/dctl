#pragma once
#include "../../node/Stack.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool, int, typename, typename Position>
struct generator
:
        // enforce static semantics
        private nonconstructible
{
        static void run(Position const& p, Stack& moves)
        {
                // no-op
        }
};

}       // namespace successor
}       // namespace dctl
