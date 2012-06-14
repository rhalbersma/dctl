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
        static void generate(Position<Rules, Board> const&, Stack&)
        {
                return;
        }

        template<template<typename, typename> class Position>
        static int count(Position<Rules, Board> const&)
        {
                return 0;
        }

        template<template<typename, typename> class Position>
        static bool detect(Position<Rules, Board> const&)
        {
                return false;
        }
};

}       // namespace successor
}       // namespace dctl
