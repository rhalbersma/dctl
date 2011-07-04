namespace board {
namespace angles {

template<int I, int A>
struct Rotate
{
        enum { VALUE = (I + A) % D360 };
};

// an inverse angle has the opposite sign modulo 360 degrees
template<int A>
struct Inverse
{
        enum { VALUE = (D360 - A) % D360 };
};

}       // namespace angles
}       // namespace board
