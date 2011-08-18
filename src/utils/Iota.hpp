namespace dctl {
namespace utils {

template <class ForwardIterator, class Size, class T>
void iota_n (ForwardIterator first, Size n, T value)
{
        for (Size i = 0; i < n; ++i)
                *first++ = value++;
}

}       // namespace utils
}       // namespace dctl
