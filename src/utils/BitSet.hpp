template<typename T, size_t N> 
BitSet<T, N>& BitSet<T, N>::flip()
{
        for (auto i = 0; i < N; ++i)
                block_[i] = ~block_[i];
        return *this;
}

template<typename T, size_t N> 
BitSet<T, N>& BitSet<T, N>::operator&= (const BitSet<T, N>& other)
{
        for (auto i = 0; i < N; ++i)
                block_[i] &= other.block_[i];
        return *this;
}

template<typename T, size_t N> 
BitSet<T, N>& BitSet<T, N>::operator|= (const BitSet<T, N>& other)
{
        for (auto i = 0; i < N; ++i)
                block_[i] |= other.block_[i];
        return *this;
}

template<typename T, size_t N> 
BitSet<T, N>& BitSet<T, N>::operator^= (const BitSet<T, N>& other)
{
        for (auto i = 0; i < N; ++i)
                block_[i] ^= other.block_[i];
        return *this;
}

template<typename T, size_t N> 
BitSet<T, N>& BitSet<T, N>::operator<<= (size_t shift)
{
        if (!shift) return *this;

        const size_t between = shift / BITS_PER_BLOCK;
        const size_t within  = shift % BITS_PER_BLOCK;

        if (!within)
                for (auto i = N - 1; i >= between; --i)
                        block_[i] = block_[i - between];
        else {
                const size_t co_within = BITS_PER_BLOCK - within;

                for (auto i = N - 1; i > between; --i)
                        block_[i] = 
                                (block_[i - between    ] <<    within) |
                                (block_[i - between - 1] >> co_within)
                        ;
                block_[between] = block_[0] << within;                                        
        }
        return *this;
}

template<typename T, size_t N> 
BitSet<T, N>& BitSet<T, N>::operator>>=(size_t shift)
{
        if (!shift) return *this;

        const size_t between = shift / BITS_PER_BLOCK;
        const size_t within  = shift % BITS_PER_BLOCK;
        const size_t co_between = N - 1 - between;

        if (!within)
                for (auto i = 0; i <= co_between; ++i)
                        block_[i] = block_[i + between];
        else {
                const size_t co_within = BITS_PER_BLOCK - within;

                for (auto i = N - 1; i < co_between; --i)
                        block_[i] = 
                                (block_[i + between    ] >>    within) |
                                (block_[i + between + 1] << co_within)
                        ;
                block_[co_between] = block_[N - 1] >> within;                                        
        }
        return *this;
}

template<typename T, size_t N> 
bool BitSet<T, N>::operator==(const BitSet<T, N>& other) const
{
        for (auto i = 0; i < N; ++i)
                if (block_[i] != other.block_[i])
                        return false;
        return true;
}

template<typename T, size_t N> 
bool BitSet<T, N>::operator!=(const BitSet<T, N>& other) const
{
        return !(*this == other);
}

template<typename T, size_t N> 
const BitSet<T, N> operator~(const BitSet<T, N>& other)
{
        return BitSet<T, N>(other).flip();
}

template<typename T, size_t N> 
const BitSet<T, N> operator&(const BitSet<T, N>& left, const BitSet<T, N>& right)
{
        return BitSet<T, N>(left) &= right;
}

template<typename T, size_t N> 
const BitSet<T, N> operator|(const BitSet<T, N>& left, const BitSet<T, N>& right)
{
        return BitSet<T, N>(left) |= right;
}

template<typename T, size_t N> 
const BitSet<T, N> operator^(const BitSet<T, N>& left, const BitSet<T, N>& right)
{
        return BitSet<T, N>(left) ^= right;
}

template<typename T, size_t N> 
const BitSet<T, N> operator<<(const BitSet<T, N>& left, size_t shift)
{
        return BitSet<T, N>(left) <<= shift;
}

template<typename T, size_t N> 
const BitSet<T, N> operator>>(const BitSet<T, N>& right, size_t shift)
{
        return BitSet<T, N>(right) >>= shift;
}
