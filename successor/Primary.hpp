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
        template<template<typename, typename> class Position>
        static void generate(const Position<Rules, Board>&, Stack&)
        {
                return;
        }

        template<template<typename, typename> class Position>
        static int count(const Position<Rules, Board>&)
        {
                return 0;
        }

        template<template<typename, typename> class Position>
        static bool detect(const Position<Rules, Board>&)
        {
                return false;
        }
};

}       // namespace successor
}       // namespace dctl
