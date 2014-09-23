#pragma once
#include <dctl/bitset/input_range_facade.hpp>     // InputRangeCoreAcces, InputRangeFacade
#include <dctl/bitset/intrinsic.hpp>              // ctznz
#include <dctl/bitset/masks.hpp>                  // none
#include <cassert>                              // assert

namespace xstd {

template<class Block, int Nb>
class InputRange;

template<class Block>
class InputRange<Block, 1>
:
        // provides member and non-member begin() / end()
        public InputRangeFacade<InputRange<Block, 1>, int, int>
{
        static_assert(is_unsigned_integer<Block>, "");

public:
        // constructors

        explicit InputRange(Block& range) noexcept
        :
                range_{range}
        {}

private:
        // gateway for InputRangeFacade to access private implementation
        friend class InputRangeCoreAccess;

        // modifiers

        void pop_front()
        {
                assert(!empty());
                range_ &= range_ - 1;
        }

        // observers

        int front() const
        {
                assert(!empty());
                return intrinsic::ctznz(range_);
        }

        // predicates

        bool empty() const noexcept
        {
                return range_ == masks::none<Block>;
        }

private:
        // representation

        Block& range_;
};

}       // namespace xstd
