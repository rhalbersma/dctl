#pragma once

//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/algo/search/objective.hpp>
#include <dctl/algo/search/root.hpp>
#include <dctl/core/model.hpp>
#include <dctl/core/state.hpp>
#include <dctl/util/type_traits.hpp>
#include <dctl/util/units.hpp>
#include <boost/test/unit_test.hpp>     // BOOST_CHECK_EQUAL
#include <string>                       // string
#include <utility>                      // pair

namespace dctl::algo {
namespace search {

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
        using rules_type = core::rules_t<State>;
        using board_type = core::board_t<State>;

        template<class Range>
        void run(Range const& tests)
        {
                for (auto const& t : tests) {
                        root_.clear_hash();
                        auto const position = core::pdn::read<rules_type, board_type>{}(t.first);
                        auto const value = root_.analyze(core::model::drop_duplicates_gen, position, t.second);
                        BOOST_WARN_EQUAL(win_value(t.second), value);
                }
        }
};

using DefaultObjective = GameObjective<NoActionsLeft>;

}       // namespace search
}       // namespace dctl::algo
