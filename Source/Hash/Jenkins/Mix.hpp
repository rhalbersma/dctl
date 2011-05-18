namespace Hash {
namespace Jenkins {

template<typename Index>
void Mix<Index>::mix(Index& index_, bool color)
{
        index_ = (0 - color) & SIDE;
}

template<typename Index>
void Mix<Index>::mix(Index& index_, BitBoard b)
{
        add_shift_L(index_, ADD_SHIFT_L[0], b);
        xor_shift_R(index_, XOR_SHIFT_R[0]);
        for (size_t i = 1; i < NUM_MIX; ++i) {
                add_shift_L(index_, ADD_SHIFT_L[i]);
                xor_shift_R(index_, XOR_SHIFT_R[i]);
        }
}

template<typename Index>
void Mix<Index>::add_shift_L(Index& index_, size_t s, BitBoard b)
{
        index_ += (b << s) - b;
}

template<typename Index>
void Mix<Index>::add_shift_L(Index& index_, size_t s)
{
        index_ += (index_ << s);
}

template<typename Index>
void Mix<Index>::xor_shift_R(Index& index_, size_t s)
{
        index_ ^= (index_ >> s);
}

}       // namespace Jenkins
}       // namespace Hash
