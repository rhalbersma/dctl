#pragma once

namespace dctl {

class Side
{
public:
        enum: bool {
                black = false,
                red = black,
                white = true,
                pass = true
        };
};

}       // namespace dctl
