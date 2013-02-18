#pragma once

namespace dctl {

struct Side
{
        enum: bool {
                black = false,
                red = black,
                white = true,
                pass = true
        };
};

}       // namespace dctl
