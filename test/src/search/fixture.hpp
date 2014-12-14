#pragma once
#include <string>                       // string
#include <utility>                      // pair
#include <boost/test/unit_test.hpp>
#include <dctl/search/root.hpp>
#include <dctl/search/objective.hpp>
#include <dctl/position/position.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules.hpp>

namespace dctl {
namespace search {

template<class Position, class Objective>
class Fixture
{
public:
        Root<Position, Objective> root_;
public:
        ~Fixture()
        {
                root_.resize_hash(1);
        }

        Fixture()
        {
                root_.resize_hash(4096);
        }

        using Test = std::pair<std::string, int>;
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;

        template<std::size_t N>
        void run(Test const (&tests)[N])
        {
                for (auto&& t : tests) {
                        root_.clear_hash();
                        auto const position = setup::read<Rules, Board, pdn::protocol>()(t.first);
                        auto const value = root_.analyze(position, t.second);
                        BOOST_CHECK_EQUAL(win_value(t.second), value);
                }
        }
};

using DefaultObjective = GameObjective<NoMovesLeft>;

}       // namespace search
}       // namespace dctl
