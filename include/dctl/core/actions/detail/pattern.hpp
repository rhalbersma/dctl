#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/board/angle.hpp>            // angle, _deg
#include <dctl/core/board/stride.hpp>           // stride_v
#include <dctl/util/type_traits.hpp>            // set_t
#include <dctl/core/rules/type_traits.hpp>      // short_ranged_tag, long_ranged_tag
#include <iterator>                             // next, prev

namespace dctl::core {

template<int Direction>
constexpr auto is_forward_v = angle{Direction} == 0_deg || 180_deg < angle{Direction};

template<int Direction, class Set>
auto find_first(Set const& s)
{
        if constexpr (is_forward_v<Direction>) {
                return s.front();
        } else {
                return s.back();
        }
}

template<class Board, int Direction, int Distance = 1>
struct advance
{
        using set_type = set_t<Board>;
        constexpr static auto n = Distance * stride_v<Board, Direction>;

        auto operator()(set_type& s) const noexcept
        {
                static_assert(0 <= n); static_assert(n < set_type::max_size());
                if constexpr (is_forward_v<Direction>) {
                        return s <<= n;
                } else {
                        return s >>= n;
                }
        }

        constexpr auto operator()(int& sq) const noexcept
        {
                if constexpr (is_forward_v<Direction>) {
                        return sq += n;
                } else {
                        return sq -= n;
                }
        }
};

template<class Board, int Direction, int Distance = 1>
struct next
{
        using set_type = set_t<Board>;
        constexpr static auto n = Distance * stride_v<Board, Direction>;

        auto operator()(set_type const& s) const noexcept
        {
                static_assert(0 <= n); static_assert(n < set_type::max_size());
                if constexpr (is_forward_v<Direction>) {
                        return s << n;
                } else {
                        return s >> n;
                }
        }

        constexpr auto operator()(int const sq) const noexcept
        {
                if constexpr (is_forward_v<Direction>) {
                        return sq + n;
                } else {
                        return sq - n;
                }
        }
};

template<class Board, int Direction, int Distance = 1>
struct prev
{
        using set_type = set_t<Board>;
        constexpr static auto n = Distance * stride_v<Board, Direction>;

        auto operator()(set_type const& s) const noexcept
        {
                static_assert(0 <= n); static_assert(n < set_type::max_size());
                if constexpr (is_forward_v<Direction>) {
                        return s >> n;
                } else {
                        return s << n;
                }
        }

        constexpr auto operator()(int const sq) const noexcept
        {
                if constexpr (is_forward_v<Direction>) {
                        return sq - n;
                } else {
                        return sq + n;
                }
        }
};

template<class Board, int Direction>
struct move_sources
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets) const noexcept
        {
                return sources & prev<Board, Direction>{}(targets);
        }
};

template<class Board, int Direction, class KingRangeCategory = short_ranged_tag>
struct move_targets
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets) const noexcept
        {
                return next<Board, Direction>{}(sources) & targets;
        }
};

template<class Board, int Direction>
struct jump_sources
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets, set_type const& empty) const noexcept
        {
                return sources & prev<Board, Direction, 1>{}(targets) & prev<Board, Direction, 2>{}(empty);
        }
};

template<class Board, int Direction, class KingRangeCategory = short_ranged_tag>
struct jump_targets
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets, set_type const& empty) const noexcept
        {
                return next<Board, Direction>{}(sources) & targets & prev<Board, Direction>{}(empty);
        }
};

template<class Board, int Direction>
struct fill
{
        using set_type = set_t<Board>;

        constexpr auto operator()(set_type generator, set_type const& propagator) const noexcept
        {
                auto flood = set_type{};
                while (!generator.empty()) {
                        flood |= generator;
                        generator = next<Board, Direction>{}(generator) & propagator;
                }
                return flood;
        }
};

template<class Board, int Direction>
struct move_targets<Board, Direction, long_ranged_tag>
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets) const noexcept
        {
                return sources ^ fill<Board, Direction>{}(sources, targets);
        }
};

template<class Board, int Direction>
struct jump_targets<Board, Direction, long_ranged_tag>
{
        using set_type = set_t<Board>;

        auto operator()(set_type const& sources, set_type const& targets, set_type const& empty) const noexcept
        {
                return
                        next<Board, Direction>{}(fill<Board, Direction>{}(sources, empty)) &
                        targets &
                        prev<Board, Direction>{}(empty)
                ;
        }
};

}       // namespace dctl::core
