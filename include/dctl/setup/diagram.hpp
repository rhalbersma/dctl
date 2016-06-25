#pragma once
#include <dctl/setup/content.hpp>
#include <dctl/setup/i_token.hpp>
#include <dctl/utility/type_traits.hpp>
#include <boost/range/irange.hpp>
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
                using namespace xstd::support_literals;

                std::stringstream ostr;
                for (auto y : boost::irange(0_zu, Board::height)) {
                        for (auto x : boost::irange(0_zu, Board::width)) {
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

}       // namespace maskup
}       // namespace dctl
