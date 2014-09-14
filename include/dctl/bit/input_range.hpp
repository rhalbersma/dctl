#pragma once
#include <dctl/bit/intrinsic.hpp>               // ctznz
#include <dctl/bit/traits.hpp>                  // none
#include <dctl/utility/input_range_facade.hpp>  // InputRangeCoreAcces, InputRangeFacade
#include <cassert>                              // assert

namespace dctl {
namespace bit {

template<class Block, int Nb>
class InputRange;

template<class Block>
class InputRange<Block, 1>
:
        // provides member and non-member begin() / end()
        public util::InputRangeFacade<InputRange<Block, 1>, int, int>
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
        friend class util::InputRangeCoreAccess;

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
                return bit::intrinsic::ctznz(range_);
        }

        // predicates

        bool empty() const noexcept
        {
                return range_ == none<Block>;
        }

private:
        // representation

        Block& range_;
};

}       // namespace bit
}       // namespace dctl
