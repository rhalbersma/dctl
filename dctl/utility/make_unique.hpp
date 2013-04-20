#pragma once

/*

Stephan T. Lavavej's make_unique proposal (N3588)
http://isocpp.org/files/papers/n3588.txt

*/

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

namespace std {
    template<class T> struct _Never_true : false_type { };

    template<class T> struct _Unique_if {
        typedef unique_ptr<T> _Single;
    };

    template<class T> struct _Unique_if<T[]> {
        typedef unique_ptr<T[]> _Runtime;
    };

    template<class T, size_t N> struct _Unique_if<T[N]> {
        static_assert(_Never_true<T>::value, "make_unique forbids T[N]. Please use T[].");
    };

    template<class T, class... Args> typename _Unique_if<T>::_Single make_unique(Args&&... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<class T> typename _Unique_if<T>::_Single make_unique_default_init() {
        return unique_ptr<T>(new T);
    }

    template<class T> typename _Unique_if<T>::_Runtime make_unique(size_t n) {
        typedef typename remove_extent<T>::type U;
        return unique_ptr<T>(new U[n]());
    }

    template<class T> typename _Unique_if<T>::_Runtime make_unique_default_init(size_t n) {
        typedef typename remove_extent<T>::type U;
        return unique_ptr<T>(new U[n]);
    }

    template<class T, class... Args> typename _Unique_if<T>::_Runtime make_unique_value_init(size_t n, Args&&... args) {
        typedef typename remove_extent<T>::type U;
        return unique_ptr<T>(new U[n]{ std::forward<Args>(args)... });
    }

    template<class T, class... Args> typename _Unique_if<T>::_Runtime make_unique_auto_size(Args&&... args) {
        typedef typename remove_extent<T>::type U;
        return unique_ptr<T>(new U[sizeof...(Args)]{ std::forward<Args>(args)... });
    }
}
