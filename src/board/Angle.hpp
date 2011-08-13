namespace dctl {
namespace board {

// partial specialization for angles
template<int I, int A>
struct Rotate<Int2Type<I>, A>
{
        // angles are subject to arithmetic modulo 360 degrees, 
        // with the unit element equal to 45 degrees anti-clockwise
        enum { value = (I + A) % Angle::D360 };
};

// an inverse angle has the opposite sign modulo 360 degrees
template<int I>
struct Inverse
{
        enum { value = (Angle::D360 - I) % Angle::D360 };
};

// mirrored forward direction index (orthogonal to the original)
template<int I>
struct MirrorUp
{
        enum { value = (I + Angle::D090) % Angle::D180 + (I / Angle::D180) * Angle::D180 };
};

// mirrored backward direction index (orthogonal to the original)
template<int I>
struct MirrorDown
{
private:
        enum {
                L = Rotate<Int2Type<I>, Angle::L090>::value,
                ML = MirrorUp<L>::value,
                RML = Rotate<Int2Type<ML>, Angle::R090>::value
        };
public:
        // M' == R M L
        enum { value = RML };
};

}       // namespace board
}       // namespace dctl
