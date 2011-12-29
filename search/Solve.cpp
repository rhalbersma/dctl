#include <memory>                       // std::unique_ptr
#include <string>                       // std::string
#include <typeinfo>                     // typeid
#include <utility>                      // std::pair
#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>
#include "../test_config.hpp"
#include "../../src/search/Root.hpp"
#include "../../src/search/Objective.hpp"
#include "../../src/node/Position.hpp"
#include "../../src/setup/Setup.hpp"
#include "../../src/board/Types.hpp"
#include "../../src/variant/Variant.hpp"

namespace dctl {
namespace search {

#if INTEGRATION_TEST == 0

typedef std::pair<std::string, int> FEN_depth;

template<typename Rules, typename Board, typename Objective = GameObjective<> >
struct Fixture
{
        Fixture() 
        {
                root_.resize_hash(26);
        }

        ~Fixture() 
        {
                root_.resize_hash(0);
        }

        int run(const FEN_depth& test_case) 
        {
                root_.clear_hash();
                const auto position = setup::read<Board, pdn::protocol>()(test_case.first);
                const auto value = root_.solve(position, test_case.second);
                //BOOST_CHECK_EQUAL(win_value(test_case.second), value);
                return value;
        };

        Root<Rules, Board, Objective> root_;
};

BOOST_AUTO_TEST_SUITE(TestRoot)

typedef boost::mpl::list
<
        variant::Checkers,
        variant::Italian,
        variant::Czech,
        variant::Thai,
        variant::Spanish,
        variant::Pool,
        variant::Russian,
        variant::International,
        variant::Killer,
        variant::Frisian
> RulesTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE(MiniInitial, R, RulesTypes)
{
        typedef board::Mini B;
        typedef NoMovesLeft TD;
        typedef Regular TS;

        typedef GameObjective<TD, TS, FirstPlayerWin> obj_1;
        typedef GameObjective<TD, TS, SecondPlayerWin> obj_2;

        typedef Fixture<R, B, obj_1> F1;
        typedef Fixture<R, B, obj_2> F2;

        std::cout << setup::diagram<B>()();
        FEN_depth test_case("W:B1,2,3,4,5,6:W13,14,15,16,17,18", 75);

        std::cout << "Solving the initial position in 6x6 draughts with rules: " << typeid(R).name() << "\n";

        std::cout << "Trying the draw rule: " << typeid(FirstPlayerWin).name() << "\n";
        auto fix_1 = std::unique_ptr<F1>(new F1);
        const auto v1 = fix_1->run(test_case);

        std::cout << "Trying the draw rule: " << typeid(SecondPlayerWin).name() << "\n";
        auto fix_2 = std::unique_ptr<F2>(new F2);
        const auto v2 = fix_2->run(test_case);

        BOOST_CHECK_GE(v1, v2);
}

BOOST_AUTO_TEST_SUITE_END()

#endif

}       // namespace search
}       // namespace dctl
