#pragma once
#include <dctl/bit/iterator/iterator_fwd.hpp>   // ConstIterator
#include <dctl/bit/iterator/reference_fwd.hpp>  // ConstReference
#include <type_traits>                          // is_convertible

namespace dctl {
namespace bit {

template<class T, class Block, int Nb>
class ConstReference
{
public:
        // constructors

        // references cannot be left uninitialized
        ConstReference() = delete;

        // references can be copied
        constexpr ConstReference(ConstReference const&) = default;

        template<class U>
        constexpr ConstReference(Block const& b, U const& value) noexcept
        :
                block_{b},
                index_{value}
        {}

        // modifiers

        // references cannot be re-assigned
        ConstReference& operator=(ConstReference const&) = delete;

        // const references cannot be assigned through
        template<class U>
        ConstReference& operator=(U const&) = delete;

        // queries

        constexpr ConstIterator<T, Block, Nb> operator&() const noexcept
        {
                return { &block_, index_ };
        }

        /* implicit */ constexpr operator T() const noexcept
        {
                return T{index_};
        }

private:
        // representation

        Block const& block_;
        int index_;
};

}       // namespace bit
}       // namespace dctl
