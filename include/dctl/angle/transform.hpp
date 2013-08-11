#pragma once
#include <dctl/angle/degrees.hpp>       // Degrees
#include <dctl/angle/transform_fwd.hpp> // inverse, rotate, mirror (partial specialization declarations)

namespace dctl {
namespace mpl {
namespace lazy {

// NOTE: because inverse, rotate and mirror are LAZY metafunctions, 
// their primary template definitions need to have been seen at this point

// partial specialization definitions

template<int Object>
struct inverse< angle::Degrees<Object> >
:
        angle::Degrees< -Object >
{};

template<int Object, int Angle>
struct rotate< angle::Degrees<Object>, angle::Degrees<Angle> >
:
        angle::Degrees< Object + Angle >
{};

template<int Object, int Axis>
struct mirror< angle::Degrees<Object>, angle::Degrees<Axis> >
:
        rotate<
                inverse<
                        rotate<
                                angle::Degrees<Object>,
                                inverse< angle::Degrees<Axis> >
                        >
                >,
                angle::Degrees<Axis>
        >
{};

}       // namespace lazy
}	// namespace mpl
}       // namespace dctl
