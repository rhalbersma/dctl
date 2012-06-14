#include <memory>                       // unique_ptr
#include <string>                       // string
#include <typeinfo>                     // typeid
#include <utility>                      // pair
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>
#include "../test_config.hpp"
#include "../../src/search/Root.hpp"
#include "../../src/search/Objective.hpp"
#include "../../src/node/Position.hpp"
#include "../../src/setup/Setup.hpp"
#include "../../src/board/Types.hpp"
#include "../../src/rules/Types.hpp"

namespace dctl {
namespace search {

#if INTEGRATION_TEST == 1

typedef std::pair<std::string, int> FEN_depth;

template<typename Rules, typename Board, typename Objective>
struct Fixture
{
        Fixture()
        {
                root_.resize_hash(27);
        }

        ~Fixture()
        {
                root_.resize_hash(0);
        }

        int run(const FEN_depth& test_case)
        {
                root_.clear_hash();
                auto const position = setup::read<Rules, Board, pdn::protocol>()(test_case.first);
                auto const value = root_.solve(position, test_case.second);
                //BOOST_CHECK_EQUAL(win_value(test_case.second), value);
                return value;
        };

        Root<Objective> root_;
};

BOOST_AUTO_TEST_SUITE(TestRoot)

typedef boost::mpl::list
<
        rules::Frisian,
        rules::Checkers,
        rules::Italian,
        rules::Czech,
        rules::Thai,
        rules::Spanish,
        rules::Pool,
        rules::Russian,
        rules::International,
        rules::Killer
> RulesTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE(MiniInitial, R, RulesTypes)
{
        typedef board::Mini B;
        typedef NoMovesLeft TD;
        typedef Misere TS;

        typedef GameObjective<TD, TS, FirstPlayerWin> obj_1;
        typedef GameObjective<TD, TS, SecondPlayerWin> obj_2;

        typedef Fixture<R, B, obj_1> F1;
        typedef Fixture<R, B, obj_2> F2;

        std::cout << setup::diagram<B>()();
        FEN_depth test_case("W:B1,2,3,4,5,6:W13,14,15,16,17,18", 75);

        std::cout << "Solving the initial position in 6x6 draughts with rules: " << typeid(R).name() << "\n";

        // block for scoping fix_1
        {
                std::cout << "Trying the draw rule: " << typeid(FirstPlayerWin).name() << "\n";
                auto fix_1 = std::unique_ptr<F1>(new F1);
                fix_1->run(test_case);
        }

        // block for scoping fix_2
        {
                std::cout << "Trying the draw rule: " << typeid(SecondPlayerWin).name() << "\n";
                auto fix_2 = std::unique_ptr<F2>(new F2);
                fix_2->run(test_case);
        }

        //BOOST_CHECK_GE(v1, v2);
}

BOOST_AUTO_TEST_SUITE_END()

#endif

}       // namespace search
}       // namespace dctl
