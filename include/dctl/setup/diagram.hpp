#pragma once
#include <dctl/setup/content.hpp>
#include <dctl/setup/i_token.hpp>
#include <dctl/utility/type_traits.hpp>
#include <range/v3/all.hpp>
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
        auto operator()(State const& p) const
        {
                using Board = board_t<State>;
                using Coord = board::Coordinates<board::origin::UpperLeft>;
                using namespace ranges;

                std::stringstream ostr;
                RANGES_FOR(auto y, view::iota(0, Board::height())) {
                        RANGES_FOR(auto x, view::iota(0, Board::width())) {
                                auto const coord = Coord{x, y};
                                if (Board::is_square(coord)) {
                                        ostr << std::setw(2) << content<Content>(p, Board::bit_from_square(Board::to_square(coord)));
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
