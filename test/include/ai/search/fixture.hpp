#pragma once
#include <dctl/ai/search/root.hpp>
#include <dctl/ai/search/objective.hpp>
#include <dctl/setup/setup.hpp>
#include <dctl/utility/units.hpp>
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL
#include <string>                       // string
#include <utility>                      // pair

namespace dctl {
namespace search {

auto const unique_gen = core::Actions<core::select::legal, true >{};

template<class State, class Objective>
class Fixture
{
public:
        Root<State, Objective> root_;
public:
        ~Fixture()
        {
                root_.resize_hash(64_KiB);
        }

        Fixture()
        {
                root_.resize_hash(64_MiB);
        }

        using Test = std::pair<std::string, int>;
        using Rules = typename State::rules_type;
        using Board = typename State::board_type;

        template<class Range>
        void run(Range const& tests)
        {
                for (auto const& t : tests) {
                        root_.clear_hash();
                        auto const position = setup::read<Rules, Board, pdn::protocol>()(t.first);
                        auto const value = root_.analyze(position, unique_gen, t.second);
                        BOOST_WARN_EQUAL(win_value(t.second), value);
                }
        }
};

using DefaultObjective = GameObjective<NoActionsLeft>;

}       // namespace search
}       // namespace dctl
