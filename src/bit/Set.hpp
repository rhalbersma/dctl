namespace dctl {
namespace bit {

template<typename T, std::size_t N> 
Set<T, N>& Set<T, N>::flip()
{
        for (auto i = 0; i < N; ++i)
                block_[i] = ~block_[i];
        return *this;
}

template<typename T, std::size_t N> 
Set<T, N>& Set<T, N>::operator&= (const Set<T, N>& other)
{
        for (auto i = 0; i < N; ++i)
                block_[i] &= other.block_[i];
        return *this;
}

template<typename T, std::size_t N> 
Set<T, N>& Set<T, N>::operator|= (const Set<T, N>& other)
{
        for (auto i = 0; i < N; ++i)
                block_[i] |= other.block_[i];
        return *this;
}

template<typename T, std::size_t N> 
Set<T, N>& Set<T, N>::operator^= (const Set<T, N>& other)
{
        for (auto i = 0; i < N; ++i)
                block_[i] ^= other.block_[i];
        return *this;
}

template<typename T, std::size_t N> 
Set<T, N>& Set<T, N>::operator<<= (std::size_t shift)
{
        if (!shift) return *this;

        const std::size_t between = shift / BITS_PER_BLOCK;
        const std::size_t within  = shift % BITS_PER_BLOCK;

        if (!within)
                for (auto i = N - 1; i >= between; --i)
                        block_[i] = block_[i - between];
        else {
                const std::size_t co_within = BITS_PER_BLOCK - within;

                for (auto i = N - 1; i > between; --i)
                        block_[i] = 
                                (block_[i - between    ] <<    within) |
                                (block_[i - between - 1] >> co_within)
                        ;
                block_[between] = block_[0] << within;                                        
        }
        return *this;
}

template<typename T, std::size_t N> 
Set<T, N>& Set<T, N>::operator>>=(std::size_t shift)
{
        if (!shift) return *this;

        const std::size_t between = shift / BITS_PER_BLOCK;
        const std::size_t within  = shift % BITS_PER_BLOCK;
        const std::size_t co_between = N - 1 - between;

        if (!within)
                for (auto i = 0; i <= co_between; ++i)
                        block_[i] = block_[i + between];
        else {
                const std::size_t co_within = BITS_PER_BLOCK - within;

                for (auto i = N - 1; i < co_between; --i)
                        block_[i] = 
                                (block_[i + between    ] >>    within) |
                                (block_[i + between + 1] << co_within)
                        ;
                block_[co_between] = block_[N - 1] >> within;                                        
        }
        return *this;
}

template<typename T, std::size_t N> 
bool Set<T, N>::operator==(const Set<T, N>& other) const
{
        for (auto i = 0; i < N; ++i)
                if (block_[i] != other.block_[i])
                        return false;
        return true;
}

template<typename T, std::size_t N> 
bool Set<T, N>::operator!=(const Set<T, N>& other) const
{
        return !(*this == other);
}

template<typename T, std::size_t N> 
const Set<T, N> operator~(const Set<T, N>& other)
{
        return Set<T, N>(other).flip();
}

template<typename T, std::size_t N> 
const Set<T, N> operator&(const Set<T, N>& left, const Set<T, N>& right)
{
        return Set<T, N>(left) &= right;
}

template<typename T, std::size_t N> 
const Set<T, N> operator|(const Set<T, N>& left, const Set<T, N>& right)
{
        return Set<T, N>(left) |= right;
}

template<typename T, std::size_t N> 
const Set<T, N> operator^(const Set<T, N>& left, const Set<T, N>& right)
{
        return Set<T, N>(left) ^= right;
}

template<typename T, std::size_t N> 
const Set<T, N> operator<<(const Set<T, N>& left, std::size_t shift)
{
        return Set<T, N>(left) <<= shift;
}

template<typename T, std::size_t N> 
const Set<T, N> operator>>(const Set<T, N>& right, std::size_t shift)
{
        return Set<T, N>(right) >>= shift;
}

}       // namespace bit
}       // namespace dctl
