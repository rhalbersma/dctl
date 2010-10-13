template<>
struct Shift<L>
{
        template<typename T> 
        T operator()(T square, size_t dir) const
        {
                return square << dir;
        }
};

template<>
struct Shift<R>
{
        template<typename T> 
        T operator()(T square, size_t dir) const
        {
                return square >> dir;
        }
};

template<>
struct ShiftAssign<L>
{
        template<typename T> 
        void operator()(T& square, size_t dir) const
        {
                square <<= dir;
        }
};

template<>
struct ShiftAssign<R>
{
        template<typename T> 
        void operator()(T& square, size_t dir) const
        {
                square >>= dir;
        }
};