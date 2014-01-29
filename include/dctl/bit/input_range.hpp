#pragma once
#include <cassert>                              // assert
#include <dctl/bit/detail/intrinsic.hpp>        // clznz
#include <dctl/utility/input_range_facade.hpp>

namespace dctl {
namespace bit {

template<class T, class Block, int Nb>
class InputRange;

template<class T, class Block>
class InputRange<T, Block, 1>
:
        public util::InputRangeFacade<InputRange<T, Block, 1>, T, T>
{
public:
        // structors

        constexpr explicit InputRange(Block& range) noexcept
        :
                range_{range}
        {}

private:
        friend class util::InputRangeCoreAccess;

        // modifiers

        constexpr void pop_front()
        {
                assert(!empty());
                range_ &= range_ - 1;
        }

        // queries

        constexpr T front() const
        {
                assert(!empty());
                return T{bit::intrinsic::ctznz(range_)};
        }

        // predicates

        constexpr bool empty() const noexcept
        {
                return range_ == 0;
        }

private:
        Block& range_;
};

}       // namespace bit
}       // namespace dctl
