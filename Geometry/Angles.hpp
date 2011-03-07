namespace Geometry {
namespace Angles {

template<size_t I, size_t A>
struct Rotate
{
        enum { VALUE = (I + A) % D360 };
};

// an inverse angle has the opposite sign modulo 360 degrees
template<size_t A>
struct Inverse
{
        enum { VALUE = (D360 - A) % D360 };
};

}       // namespace Angles
}       // namespace Geometry
