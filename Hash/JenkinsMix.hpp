template<typename Index>
void JenkinsMix<Index>::mix(Index& d_index, bool color)
{
        d_index = (0 - color) & SIDE;
}

template<typename Index>
void JenkinsMix<Index>::mix(Index& d_index, BitBoard b)
{
        add_shift_L(d_index, ADD_SHIFT_L[0], b);
        xor_shift_R(d_index, XOR_SHIFT_R[0]);
        for (size_t i = 1; i < NUM_MIX; ++i) {
                add_shift_L(d_index, ADD_SHIFT_L[i]);
                xor_shift_R(d_index, XOR_SHIFT_R[i]);
        }
}

template<typename Index>
void JenkinsMix<Index>::add_shift_L(Index& d_index, size_t s, BitBoard b)
{
        d_index += (b << s) - b;
}

template<typename Index>
void JenkinsMix<Index>::add_shift_L(Index& d_index, size_t s)
{
        d_index += (d_index << s);
}

template<typename Index>
void JenkinsMix<Index>::xor_shift_R(Index& d_index, size_t s)
{
        d_index ^= (d_index >> s);
}
