#pragma once
#include <dctl/angle/degrees.hpp>       // Degrees, inverse, rotate, mirror (constexpr function templates)
#include <dctl/angle/transform_fwd.hpp> // inverse, rotate, mirror (partial specialization declarations)

namespace dctl {
namespace mpl {
namespace lazy {

// NOTE: because inverse, rotate and mirror are LAZY metafunctions, 
// their primary template definitions need to have been seen at this point

// partial specialization definitions

template<int Alpha>
struct inverse< angle::Degrees<Alpha> >
:
        angle::Degrees< angle::inverse(Alpha) >
{};

template<int Alpha, int Theta>
struct rotate< angle::Degrees<Alpha>, angle::Degrees<Theta> >
:
        angle::Degrees< angle::rotate(Alpha, Theta) >
{};

template<int Alpha, int Theta>
struct mirror< angle::Degrees<Alpha>, angle::Degrees<Theta> >
:
        angle::Degrees< angle::mirror(Alpha, Theta) >
{};

}       // namespace lazy
}	// namespace mpl
}       // namespace dctl
