#pragma once
#include <cstddef>                      // std::size_t
#include <type_traits>                  // 

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

        typedef std::size_t       size_type;
        typedef T                 value_type;
        typedef value_type*       pointer;
        typedef value_type const* const_pointer;
        typedef value_type&       reference;
        typedef value_type const& const_reference;
        typedef ptrdiff_t         difference_type;

private:
        // representation

        typename std::aligned_storage<N * sizeof(value_type), 16>::type buf_;
        pointer ptr_;

        const_pointer buffer_cbegin() const
        {
                return reinterpret_cast<const_pointer>(&buf_);
        }

        pointer buffer_begin() const
        {
                return const_cast<pointer>(buffer_cbegin());
        }

        const_pointer buffer_cend() const
        {
                return buffer_cbegin() + N;
        }
        
public:
        // structors

        stack_alloc() 
        : 
		buf_(),
                ptr_(buffer_begin()) 
        {}

        stack_alloc(stack_alloc const&) 
        : 
		buf_(),
                ptr_(buffer_begin()) 
        {}
        
        template <class U> 
        stack_alloc(stack_alloc<U, N> const&)
        : 
		buf_(),
                ptr_(buffer_begin()) 
        {}

        template <class U> 
        struct rebind 
        {
                typedef stack_alloc<U, N> other;
        };

private:
        stack_alloc& operator=(stack_alloc const&);

public:
        pointer allocate(size_type n, typename stack_alloc<void, N>::const_pointer = 0)
        {
                if (ptr_ + n <= buffer_cend()) {
                        auto const pre = ptr_;
                        ptr_ += n;
                        return pre;
                }
                return static_cast<pointer>(::operator new(n * sizeof(value_type)));
        }

        void deallocate(pointer p, size_type n)
        {
                if (buffer_cbegin() <= p && p < buffer_cend()) {
                        if (p + n == ptr_)
                                ptr_ = p;
                } else {
                        ::operator delete(p);
                }
        }

        size_type max_size() const 
        {
                return size_type(~0) / sizeof(value_type);
        }

        void construct(pointer p)
        {
                ::new((void*)p) value_type();
        }

        template <class Arg0>
        void construct(pointer p, Arg0 const& arg0)
        {
                ::new((void*)p) value_type(arg0);
        }

        template <class Arg0, class Arg1>
        void construct(pointer p, Arg0 const& arg0, Arg1 const& arg1)
        {
                ::new((void*)p) value_type(arg0, arg1);
        }

        template <class Arg0, class Arg1, class Arg2>
        void construct(pointer p, Arg0 const& arg0, Arg1 const& arg1, Arg2 const& arg2)
        {
                ::new((void*)p) value_type(arg0, arg1, arg2);
        }

        // A compiler bug causes it to believe that p->~T() doesn't reference p.
        // http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx 
        void destroy(pointer const p) const 
        {
                p->~T();
        }

        bool operator==(stack_alloc& other) const 
        {
                return &buf_ == &other.buf_;
        }
        
        bool operator!=(stack_alloc& other) const 
        {
                return !(*this == other); 
        }
};
