#pragma once
#include <dctl/angle/Degrees.hpp>       // Degrees
#include <dctl/mpl/transform.hpp>       // inverse, mirror, rotate (primary template definitions)

namespace dctl {
namespace mpl {

// partial specialization declarations

template<int N>
struct inverse< angle::Degrees<N> >;

template<int N1, int N2>
struct rotate< angle::Degrees<N1>, angle::Degrees<N2> >;

template<int N1, int N2>
struct mirror< angle::Degrees<N1>, angle::Degrees<N2> >;

}	// namespace mpl
}       // namespace dctl
