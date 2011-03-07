#include "Angles.h"

namespace Geometry {
namespace Squares {

// identity rotation
template<typename In>
struct Rotate<In, Angles::D000>
{
        typedef In Out;
};

// rotate 90 degrees right
template<typename In>
struct Rotate<In, Angles::D270>
{
        typedef Grid<In::WIDTH, In::HEIGHT, (In::WIDTH % 2) ^ !In::PARITY> Out;
};

// rotate 90 degree left
template<typename In>
struct Rotate<In, Angles::D090>
{
        typedef Grid<In::WIDTH, In::HEIGHT, (In::HEIGHT % 2) ^ !In::PARITY> Out;
};

// rotate 180 degrees
template<typename In>
struct Rotate<In, Angles::D180>
{
        typedef Grid<In::HEIGHT, In::WIDTH, (In::HEIGHT % 2) ^ (In::WIDTH % 2) ^ In::PARITY> Out;
};

}       // namespace Squares
}       // namespace Geometry
