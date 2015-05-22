#pragma once
#include <dctl/search/root.hpp>
#include <dctl/search/objective.hpp>
#include <dctl/position/position.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules.hpp>
#include <dctl/utility/units.hpp>
#include <boost/test/unit_test.hpp>
#include <string>                       // string
#include <utility>                      // pair

namespace dctl {
namespace search {

auto const unique_gen = successor::Successor<successor::select::legal, true >{};

template<class Position, class Objective>
class Fixture
{
public:
        Root<Position, Objective> root_;
public:
        ~Fixture()
        {
                root_.resize_hash(65_KiB);
        }

        Fixture()
        {
                root_.resize_hash(4_GiB);
        }

        using Test = std::pair<std::string, int>;
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;

        template<std::size_t N>
        void run(Test const (&tests)[N])
        {
                for (auto const& t : tests) {
                        root_.clear_hash();
                        auto const position = setup::read<Rules, Board, pdn::protocol>()(t.first);
                        auto const value = root_.analyze(position, unique_gen, t.second);
                        BOOST_CHECK_EQUAL(win_value(t.second), value);
                }
        }
};

using DefaultObjective = GameObjective<NoMovesLeft>;

}       // namespace search
}       // namespace dctl
