#pragma once
#include <cstddef>
#include <cassert>
#include <dctl/guarded/default_delete.hpp>
#include <dctl/guarded/noexcept.hpp>

template <std::size_t N>
class arena
{
    static const std::size_t alignment = 16;
    /*alignas(alignment)*/ char buf_[N];
    char* ptr_;

    std::size_t 
    align_up(std::size_t n) DCTL_PP_NOEXCEPT
        {return n + (alignment-1) & ~(alignment-1);}

    bool
    pointer_in_buffer(char* p) DCTL_PP_NOEXCEPT
        {return buf_ <= p && p <= buf_ + N;}

public:
    arena() DCTL_PP_NOEXCEPT : ptr_(buf_) {}
    ~arena() {ptr_ = nullptr;}
    arena(const arena&) DCTL_PP_IS_DELETE
    arena& operator=(const arena&) DCTL_PP_IS_DELETE

    char* allocate(std::size_t n);
    void deallocate(char* p, std::size_t n) DCTL_PP_NOEXCEPT;

    static /*constexpr*/ std::size_t size() {return N;}
    std::size_t used() const {return static_cast<std::size_t>(ptr_ - buf_);}
    void reset() {ptr_ = buf_;}
};

template <std::size_t N>
char*
arena<N>::allocate(std::size_t n)
{
    assert(pointer_in_buffer(ptr_) && "short_alloc has outlived arena");
    n = align_up(n);
    if (buf_ + N - ptr_ >= n)
    {
        char* r = ptr_;
        ptr_ += n;
        return r;
    }
    return static_cast<char*>(::operator new(n));
}

template <std::size_t N>
void
arena<N>::deallocate(char* p, std::size_t n) DCTL_PP_NOEXCEPT
{
    assert(pointer_in_buffer(ptr_) && "short_alloc has outlived arena");
    if (pointer_in_buffer(p))
    {
        n = align_up(n);
        if (p + n == ptr_)
            ptr_ = p;
    }
    else
        ::operator delete(p);
}

template <class T, std::size_t N>
class short_alloc
{
    arena<N>& a_;
public:
    typedef T value_type;

public:
    template <class _Up> struct rebind {typedef short_alloc<_Up, N> other;};

    short_alloc(arena<N>& a) DCTL_PP_NOEXCEPT : a_(a) {}
    template <class U>
        short_alloc(const short_alloc<U, N>& a) DCTL_PP_NOEXCEPT
            : a_(a.a_) {}
    //short_alloc(const short_alloc&) = default;
    short_alloc& operator=(const short_alloc&) DCTL_PP_IS_DELETE

    T* allocate(std::size_t n)
    {
        return reinterpret_cast<T*>(a_.allocate(n*sizeof(T)));
    }
    void deallocate(T* p, std::size_t n) DCTL_PP_NOEXCEPT
    {
        a_.deallocate(reinterpret_cast<char*>(p), n*sizeof(T));
    }

    template <class T1, std::size_t N1, class U, std::size_t M>
    friend
    bool
    operator==(const short_alloc<T1, N1>& x, const short_alloc<U, M>& y) DCTL_PP_NOEXCEPT;

    template <class U, std::size_t M> friend class short_alloc;
};

template <class T, std::size_t N, class U, std::size_t M>
inline
bool
operator==(const short_alloc<T, N>& x, const short_alloc<U, M>& y) DCTL_PP_NOEXCEPT
{
    return N == M && &x.a_ == &y.a_;
}

template <class T, std::size_t N, class U, std::size_t M>
inline
bool
operator!=(const short_alloc<T, N>& x, const short_alloc<U, M>& y) DCTL_PP_NOEXCEPT
{
    return !(x == y);
}
