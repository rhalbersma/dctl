#pragma once
#include <cstddef>      // size_t

namespace dctl {
namespace detail {

template<class Board>
class Quality
{
        std::size_t num_captured_kings_;
public:
        Quality() = default;

        template<class Tracker>
        explicit constexpr Quality(Tracker const& t) noexcept
        :
                num_captured_kings_{t.captured(Piece::king).count()}
        {}

        constexpr auto num_captured_kings() const noexcept
        {
                return num_captured_kings_;
        }
};

}       // namespace detail
}       // namespace dctl
