#pragma once
#include "Enumerator_fwd.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool, int, typename, typename Position>
struct enumerator
:
        // enforce static semantics
        private nonconstructible
{
        static int run(Position const&)
        {
                return 0;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
