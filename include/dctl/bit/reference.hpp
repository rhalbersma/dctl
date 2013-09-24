#pragma once
#include <type_traits>                          // is_convertible
#include <dctl/bit/iterator_fwd.hpp>            // bit_iterator
#include <dctl/bit/reference_fwd.hpp>           // bit_reference

namespace dctl {
namespace bit {

template<class T, class WordT, int Nw>
class bit_reference
{
public:
        // structors

        template<class U>
        constexpr bit_reference(WordT const& s, U u) noexcept
        :
                segment_{s},
                index_{static_cast<T>(u)}
        {
                static_assert(std::is_convertible<U, T>::value, "");
        }

        // modifiers

        bit_reference& operator=(bit_reference const&) = delete;

        // views

        constexpr operator T() const noexcept
        {
                return index_;
        }

        constexpr auto operator&() const noexcept -> bit_iterator<T, WordT, Nw>
        {
                return {&segment_, index_};
        }

private:
        // representation

        WordT const& segment_;
        T index_;
};

}       // namespace bit
}       // namespace dctl
