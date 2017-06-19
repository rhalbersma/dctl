#pragma once
#include <cctype>       // tolower, toupper

namespace dctl::core {
namespace detail {

template<char Black, char White, char Empty>
struct token_interface
{
        constexpr static auto black = Black;
        constexpr static auto white = White;
        constexpr static auto empty = Empty;

        constexpr static char color[] =
        {
                black,
                white
        };

        inline static char const lower[] =
        {
                static_cast<char>(std::tolower(black)),
                static_cast<char>(std::tolower(white))
        };

        inline static char const upper[] =
        {
               static_cast<char>(std::toupper(black)),
               static_cast<char>(std::toupper(white))
        };
};

}       // namespace detail
}       // namespace dctl::core
