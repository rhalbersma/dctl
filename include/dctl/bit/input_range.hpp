#pragma once
#include <cassert>                              // assert
#include <dctl/bit/detail/intrinsic.hpp>        // ctznz
#include <dctl/utility/input_range_facade.hpp>  // InputRangeCoreAcces, InputRangeFacade

namespace dctl {
namespace bit {

template<class T, class Block, int Nb>
class InputRange;

template<class T, class Block>
class InputRange<T, Block, 1>
:
        // provides member and non-member begin() / end()
        public util::InputRangeFacade<InputRange<T, Block, 1>, T, T>
{
public:
        // structors

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

        // queries

        auto front() const
        {
                assert(!empty());
                return T{bit::intrinsic::ctznz(range_)};
        }

        // predicates

        auto empty() const noexcept
        {
                return range_ == 0;
        }

private:
        // representation

        Block& range_;
};

}       // namespace bit
}       // namespace dctl
