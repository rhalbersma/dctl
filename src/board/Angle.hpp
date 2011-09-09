namespace dctl {
namespace board {

// partial specialization for angles
template<int I, int A>
struct rotate<Int2Type<I>, A>
{
        // angles are subject to arithmetic modulo 360 degrees, 
        // with the unit element equal to 45 degrees anti-clockwise
        BOOST_STATIC_CONSTANT(auto, value = 
                (I + A) % Angle::D360
        );
};

// an inverse angle has the opposite sign modulo 360 degrees
template<int I>
struct inverse
{
        BOOST_STATIC_CONSTANT(auto, value = 
                (Angle::D360 - I) % Angle::D360
        );
};

// mirrored forward direction index (orthogonal to the original)
template<int I>
struct mirror_up
{
        BOOST_STATIC_CONSTANT(auto, value = 
                (I + Angle::D090) % Angle::D180 + (I / Angle::D180) * Angle::D180
        );
};

// mirrored backward direction index (orthogonal to the original)
template<int I>
struct mirror_down
{
private:
        // NOTE: parenthesize rotate<..., ...> to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, L = (rotate<Int2Type<I>, Angle::L090>::value));
        BOOST_STATIC_CONSTANT(auto, ML = mirror_up<L>::value);
        BOOST_STATIC_CONSTANT(auto, RML = (rotate<Int2Type<ML>, Angle::R090>::value));

public:
        // M' == R M L
        BOOST_STATIC_CONSTANT(auto, value = RML);
};

}       // namespace board
}       // namespace dctl
