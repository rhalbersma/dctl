#pragma once
#include <dctl/angle/degrees.hpp>       // Degrees
#include <dctl/mpl/transform.hpp>       // inverse, mirror, rotate (primary template definitions)

namespace dctl {
namespace mpl {
namespace lazy {

// partial specialization declarations

template<int Alpha>
struct inverse< angle::Degrees<Alpha> >;

template<int Alpha, int Theta>
struct rotate< angle::Degrees<Alpha>, angle::Degrees<Theta> >;

template<int Alpha, int Theta>
struct mirror< angle::Degrees<Alpha>, angle::Degrees<Theta> >;

}       // namespace lazy
}	// namespace mpl
}       // namespace dctl
