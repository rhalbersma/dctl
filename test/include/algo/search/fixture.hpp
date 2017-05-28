#pragma once
#include <dctl/algo/search/objective.hpp>
#include <dctl/algo/search/root.hpp>
#include <dctl/core/setup/setup.hpp>
#include <dctl/util/units.hpp>
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL
#include <string>                       // string
#include <utility>                      // pair

namespace dctl {
namespace aima {
namespace search {

auto const drop_duplicates_gen = Actions<select::legal, drop_duplicates_tag>{};

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
        using rules_type = rules_t<State>;
        using board_type = board_t<State>;

        template<class Range>
        void run(Range const& tests)
        {
                for (auto const& t : tests) {
                        root_.clear_hash();
                        auto const position = setup::read<rules_type, board_type, pdn::protocol>()(t.first);
                        auto const value = root_.analyze(drop_duplicates_gen, position, t.second);
                        BOOST_WARN_EQUAL(win_value(t.second), value);
                }
        }
};

using DefaultObjective = GameObjective<NoActionsLeft>;

}       // namespace search
}       // namespace aima
}       // namespace dctl
