#pragma once
#include <dctl/core/setup/content.hpp>
#include <dctl/core/setup/i_token.hpp>
#include <dctl/util/type_traits.hpp>
#include <iomanip>                      // setw
#include <sstream>                      // stringstream
#include <string>                       // string

namespace dctl {
namespace setup {

// primary template definition
template
<
        class Protocol, class Content = typename Token<Protocol>::type
>
struct diagram
{
public:
        // position content in diagram layout
        template<class State>
        auto operator()(State const& s) const
        {
                using Board = board_t<State>;
                using Coord = board::detail::coordinates<board::detail::upper_left>;

                std::stringstream ostr;
                for (auto y = 0; y < Board::height; ++y) {
                        for (auto x = 0; x < Board::width; ++x) {
                                auto const coord = Coord{x, y};
                                if (Board::is_square(coord)) {
                                        ostr << std::setw(2) << content<Content>(s, Board::bit_from_square(Board::to_square(coord)));
                                } else {
                                        ostr << std::string(2, ' ');
                                }
                        }
                        ostr << '\n';
                }
                return ostr.str();
        }
};

}       // namespace setup
}       // namespace dctl
