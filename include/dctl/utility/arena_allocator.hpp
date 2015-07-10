#pragma once
#include <cassert>
#include <cstddef>
#include <type_traits>
#include <boost/operators.hpp>

namespace dctl {

// adapted from Howard Hinnant's short allocator
// http://home.roadrunner.com/~hinnant/stack_alloc.html

template <std::size_t N>
class arena
{
public:
        ~arena()
        {
                reset();
        }

        arena() noexcept
        :
                ptr_(buf_)
        {}

        arena(arena const&) = delete;
        arena& operator=(arena const&) = delete;

        static constexpr std::size_t size()
        {
                return N;
        }

        std::size_t used() const
        {
                return static_cast<std::size_t>(ptr_ - buf_);
        }

        void reset()
        {
                ptr_ = nullptr;
        }

        char* allocate(std::size_t n)
        {
                assert(pointer_in_buffer(ptr_) && "short_alloc has outlived arena");
                n = align_up(n);
                if (ptr_ + n <= buf_ + N) {
                        char* r = ptr_;
                        ptr_ += n;
                        return r;
                }
                return static_cast<char*>(::operator new(n));
        }

        void deallocate(char* p, std::size_t n) noexcept
        {
                assert(pointer_in_buffer(ptr_) && "short_alloc has outlived arena");
                if (pointer_in_buffer(p)) {
                        n = align_up(n);
                        if (p + n == ptr_)
                                ptr_ = p;
                } else {
                        ::operator delete(p);
                }
        }

private:
        std::size_t align_up(std::size_t n) noexcept
        {
                return (n + (alignment - 1)) & ~(alignment - 1);
        }

        bool pointer_in_buffer(char* p) noexcept
        {
                return buf_ <= p && p <= buf_ + N;
        }

        static const std::size_t alignment = 16;
        alignas(alignment) char buf_[N];
        char* ptr_;
};

template <class T, std::size_t N>
class short_alloc
:
        boost::equality_comparable< short_alloc<T, N> > // ==, !=
{
public:
        using value_type = T;
        static auto constexpr num_bytes = N * sizeof(T);

        template <class U>
        struct rebind
        {
                using other = short_alloc<U, N>;
        };

        short_alloc(arena<num_bytes>& a) noexcept
        :
                a_(a)
        {}

        template <class U>
        short_alloc(short_alloc<U, N> const& a) noexcept
        :
                a_(a.a_)
        {}

        short_alloc(short_alloc const&) = default;
        short_alloc& operator=(short_alloc const&) = delete;

        T* allocate(std::size_t n)
        {
                return reinterpret_cast<T*>(a_.allocate(n * sizeof(T)));
        }

        void deallocate(T* p, std::size_t n) noexcept
        {
                a_.deallocate(reinterpret_cast<char*>(p), n * sizeof(T));
        }

        // operator!= provided by boost::equality_comparible
        template<class U, std::size_t M>
        friend bool operator==(short_alloc<T, N> const& x, short_alloc<U, M> const& y) noexcept
        {
                return (N * sizeof(T) == M * sizeof(U)) && &x.a_ == &y.a_;
        }

        template <class U, std::size_t M>
        friend class short_alloc;

private:
        arena<num_bytes>& a_;

};

}       // namespace dctl
