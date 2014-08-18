#pragma once
#include <dctl/bit/iterator/iterator_fwd.hpp>   // ConstIterator
#include <dctl/bit/iterator/reference_fwd.hpp>  // ConstReference
#include <cstddef>                              // size_t

namespace dctl {
namespace bit {

template<class Block, std::size_t Nb>
class ConstReference
{
public:
        // constructors

        // references cannot be left uninitialized
        ConstReference() = delete;

        constexpr ConstReference(Block const& b, std::size_t n) noexcept
        :
                block_{b},
                index_{n}
        {}

        // copying and assignment

        // references can be copied
        constexpr ConstReference(ConstReference const&) = default;

        // references cannot be assigned to
        ConstReference& operator=(ConstReference const&) = delete;

        // const references cannot be assigned through
        ConstReference& operator=(std::size_t) = delete;

        // observers

        constexpr ConstIterator<Block, Nb> operator&() const noexcept
        {
                return { &block_, index_ };
        }

        /* implicit */ constexpr operator int() const noexcept
        {
                return static_cast<int>(index_);
        }

private:
        // representation

        Block const& block_;
        std::size_t index_;
};

}       // namespace bit
}       // namespace dctl
