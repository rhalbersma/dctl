#pragma once
#include <stdexcept>                    // logic_error
#include <dctl/angle/angle.hpp>         // make_angle
#include <dctl/angle/degrees.hpp>       // D00, D045, D090, D135, D180, D225, D270, D315

namespace dctl {
namespace grid {

// primary template definition
template<class Grid>
class Shift
{
private:
        // diagonal directions

        static constexpr auto left_down = (Grid::width + Grid::num_columns) / 2;
        static constexpr auto right_down = left_down + 1;

        // orthogonal directions

        static constexpr auto right = right_down - left_down;
        static constexpr auto down = right_down + left_down;

        // equivalent directions

        static constexpr auto left_up = right_down;
        static constexpr auto right_up = left_down;
        static constexpr auto left = right;
        static constexpr auto up = down;

public:
        static constexpr auto size(int direction)
        {
                switch(dctl::make_angle(direction)) {
                case angle::D000: return right     ;
                case angle::D045: return right_up  ;
                case angle::D090: return up        ;
                case angle::D135: return left_up   ;
                case angle::D180: return left      ;
                case angle::D225: return left_down ;
                case angle::D270: return down      ;
                case angle::D315: return right_down;
                default: return throw std::logic_error("Shift directions shall be a multiple of 45 degrees"), direction;
                }
        }
};

}       // namespace grid
}       // namespace dctl
