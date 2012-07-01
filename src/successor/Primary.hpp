#pragma once
#include "Driver_fwd.hpp"
#include "../node/Stack.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template
<
        bool Color,
        int Material,
        typename Selection,
        typename Rules,
        typename Board
>
struct Driver
:
        // enforce static semantics
        private nonconstructible
{
        template<typename Position>
        static void generate(Position const&, Stack&)
        {
                // no-op
        }

        template<typename Position>
        static int count(Position const&)
        {
                return 0;
        }

        template<typename Position>
        static bool detect(Position const&)
        {
                return false;
        }
};

}       // namespace successor
}       // namespace dctl
