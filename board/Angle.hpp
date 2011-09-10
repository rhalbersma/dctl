namespace dctl {

// partial specialization for angles
template<int Index, int Theta>
struct rotate<Angle<Index>, Theta>
{
        // angles are subject to arithmetic modulo 360 degrees, 
        // with the unit element equal to 45 degrees anti-clockwise
        typedef Angle<(Index + Theta) % Degrees::D360> type;
};

// an inverse angle has the opposite sign modulo 360 degrees
template<int Index>
struct inverse
{
        BOOST_STATIC_CONSTANT(auto, value = 
                (Degrees::D360 - Index) % Degrees::D360
        );
};

// mirrored forward direction index (orthogonal to the original)
template<int Index>
struct mirror_up
{
        BOOST_STATIC_CONSTANT(auto, value = 
                (Index + Degrees::D090) % Degrees::D180 + (Index / Degrees::D180) * Degrees::D180
        );
};

// mirrored backward direction index (orthogonal to the original)
template<int I>
struct mirror_down
{
private:
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, L = (rotate<Angle<I>, Degrees::L090>::type::index));
        BOOST_STATIC_CONSTANT(auto, ML = mirror_up<L>::value);
        BOOST_STATIC_CONSTANT(auto, RML = (rotate<Angle<ML>, Degrees::R090>::type::index));

public:
        // M' == R M L
        BOOST_STATIC_CONSTANT(auto, value = RML);
};

}       // namespace dctl
