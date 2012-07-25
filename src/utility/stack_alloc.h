#pragma once
#include <cstddef>                      // size_t, ptrdiff_t
#include <type_traits>                  // aligned_storage, alignment_of

template
<
        class T, 
        std::size_t N
> 
class stack_alloc;

template<std::size_t N>
class stack_alloc<void, N>
{
public:
        typedef void const* const_pointer;
        typedef void        value_type;
};

template<class T, std::size_t N>
class stack_alloc
{
public:
        // typedefs

        typedef T                 value_type;
        typedef value_type*       pointer;
        typedef value_type const* const_pointer;
        typedef value_type&       reference;
        typedef value_type const& const_reference;

        typedef std::size_t       size_type;
        typedef std::ptrdiff_t    difference_type;

        template <class U> 
        struct rebind 
        {
                typedef stack_alloc<U, N> other;
        };

private:
        // representation

        typename std::aligned_storage<N * sizeof(value_type), std::alignment_of<value_type>::value>::type buf_;
        pointer ptr_;

private:
        // buffer access

        const_pointer cbegin() const
        {
                return reinterpret_cast<const_pointer>(&buf_);
        }

        const_pointer cend() const
        {
                return cbegin() + N;
        }

        pointer begin() const
        {
                return const_cast<pointer>(cbegin());
        }

        pointer end() const
        {
                return const_cast<pointer>(cend());
        }
        
public:
        // structors

        stack_alloc()
        : 
		buf_(),
                ptr_(begin()) 
        {}

        stack_alloc(stack_alloc const&)
        : 
		buf_(),
                ptr_(begin()) 
        {}
        
        template <class U> 
        stack_alloc(stack_alloc<U, N> const&)
        : 
		buf_(),
                ptr_(begin()) 
        {}

private:
        stack_alloc& operator=(stack_alloc const&);

public:
        size_type max_size() const
        {
                return size_type(~0) / sizeof(value_type);
        }

        pointer allocate(size_type n, typename stack_alloc<void, N>::const_pointer = 0)
        {
                if (ptr_ + n <= cend()) {
                        auto const pre = ptr_;
                        ptr_ += n;
                        return pre;
                }
                return static_cast<pointer>(::operator new(n * sizeof(value_type)));
        }

        void deallocate(pointer p, size_type n)
        {
                if (cbegin() <= p && p < cend()) {
                        if (p + n == ptr_)
                                ptr_ = p;
                } else {
                        ::operator delete(p);
                }
        }

        // TODO: use C++11 variadic templates here
        /*
        template<typename... Args>
        void construct(Args&&... args)
        {
                ::new((void*)p) value_type(std::forward<Args>(args)...);
        }
        */

        void construct(pointer p)
        {
                ::new((void*)p) value_type();
        }

        template <class Arg0>
        void construct(pointer p, Arg0&& arg0)
        {
                ::new((void*)p) value_type(std::forward<Arg0>(arg0));
        }

        template <class Arg0, class Arg1>
        void construct(pointer p, Arg0&& arg0, Arg1&& arg1)
        {
                ::new((void*)p) value_type(std::forward<Arg0>(arg0), std::forward<Arg1>(arg1));
        }

        template <class Arg0, class Arg1, class Arg2>
        void construct(pointer p, Arg0&& arg0, Arg1&& arg1, Arg2&& arg2)
        {
                ::new((void*)p) value_type(std::forward<Arg0>(arg0), std::forward<Arg1>(arg1), std::foward<Arg2>(arg2));
        }

        void destroy(pointer const p) const 
        {
                p->~T();
        }

        friend bool operator==(stack_alloc& lhs, stack_alloc& rhs) 
        {
                return &lhs.buf_ == &rhs.buf_;
        }
        
        friend bool operator!=(stack_alloc& lhs, stack_alloc& rhs) 
        {
                return !(lhs == rhs); 
        }
};
