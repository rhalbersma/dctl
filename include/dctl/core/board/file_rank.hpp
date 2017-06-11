#pragma once
#include <dctl/core/board/detail/set_filter.hpp>        // set_filter
#include <dctl/core/board/coordinates.hpp>              // to_llo
#include <xstd/type_traits.hpp>                         // to_underlying_type
#include <array>                                        // array
#include <cassert>                                      // assert
#include <cstddef>                                      // size_t

namespace dctl::core {

template<class Board>
class file
{
        template<color Side>
        struct init
        {
                constexpr auto operator()() const noexcept
                {
                        auto result = std::array<set_t<Board>, Board::width>{};
                        for (auto f = 0; f < Board::width; ++f) {
                                result[static_cast<std::size_t>(f)] = detail::set_filter<Board>([=](auto const sq){
                                        assert(0 <= f); assert(f < Board::width);
                                        return to_llo(sq, Board::inner_grid).x == (Side == color::white ? f : Board::width - 1 - f);
                                });
                        }
                        return result;
                }
        };

        constexpr static auto table = std::array<std::array<set_t<Board>, Board::width>, 2>
        {{
                init<color::black>{}(),
                init<color::white>{}()
        }};
public:
        constexpr auto operator()(color const c, int const f) const // Throws: Nothing.
        {
                assert(static_cast<std::size_t>(f) < Board::width);
                return table[xstd::to_underlying_type(c)][static_cast<std::size_t>(f)];
        }
};

template<class Board>
class rank
{
        template<color Side>
        struct init
        {
                constexpr auto operator()() const noexcept
                {
                        auto result = std::array<set_t<Board>, Board::height>{};
                        for (auto r = 0; r < Board::height; ++r) {
                                result[static_cast<std::size_t>(r)] = detail::set_filter<Board>([=](auto const sq) {
                                        assert(0 <= r); assert(r < Board::height);
                                        return to_llo(sq, Board::inner_grid).y == (Side == white_c ? r : Board::height - 1 - r);
                                });
                        }
                        return result;
                }
        };

        constexpr static auto table = std::array<std::array<set_t<Board>, Board::height>, 2>
        {{
                init<color::black>{}(),
                init<color::white>{}()
        }};
public:
        constexpr auto operator()(color const c, int const r) const // Throws: Nothing.
        {
                assert(static_cast<std::size_t>(r) < Board::height);
                return table[xstd::to_underlying_type(c)][static_cast<std::size_t>(r)];
        }
};

}       // namespace dctl::core
