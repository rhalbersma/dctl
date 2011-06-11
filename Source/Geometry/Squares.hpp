#include "Angles.h"

namespace geometry {
namespace squares {

// identity rotation
template<typename In>
struct Rotate<In, angles::D000>
{
        typedef In Out;
};

// rotate 90 degrees right
template<typename In>
struct Rotate<In, angles::D270>
{
        typedef Grid
        <
                In::WIDTH, 
                In::HEIGHT, 
                (In::WIDTH % 2) ^ !In::PARITY
        > Out;
};

// rotate 90 degree left
template<typename In>
struct Rotate<In, angles::D090>
{
        typedef Grid
        <
                In::WIDTH, 
                In::HEIGHT, 
                (In::HEIGHT % 2) ^ !In::PARITY
        > Out;
};

// rotate 180 degrees
template<typename In>
struct Rotate<In, angles::D180>
{
        typedef Grid
        <
                In::HEIGHT, 
                In::WIDTH, 
                (In::HEIGHT % 2) ^ (In::WIDTH % 2) ^ In::PARITY
        > Out;
};

}       // namespace squares
}       // namespace geometry
