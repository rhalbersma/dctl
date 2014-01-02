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

        // structors

        Side() = default;

        explicit Side(bool color): to_move_{color} {}

        // modifiers

        void flip()
        {
                to_move_ ^= pass;
        }

        // queries

        operator bool() const
        {
                return to_move_;
        }

private:
        // representation

        bool to_move_{};
};

}       // namespace dctl
