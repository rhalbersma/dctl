#include <string>                       // string
#include <utility>                      // pair
#include <boost/test/unit_test.hpp>
#include "../../src/search/Root.hpp"
#include "../../src/search/Objective.hpp"
#include "../../src/node/Position.hpp"
#include "../../src/setup/Setup.hpp"
#include "../../src/board/Types.hpp"
#include "../../src/rules/Types.hpp"

namespace dctl {
namespace search {

template<typename Objective>
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

        typedef std::pair<std::string, int> FEN_depth;

        template<typename Rules, typename Board, std::size_t N>
        void run(FEN_depth const (&tests)[N])
        {
                for (auto it = std::begin(tests); it != std::end(tests); ++it) {
                        root_.clear_hash();
                        auto const position = setup::read<Rules, Board, pdn::protocol>()(it->first);
                        auto const value = root_.analyze(position, it->second);
                        BOOST_CHECK_EQUAL(win_value(it->second), value);
                }
        };

        Root<Objective> root_;
};

typedef GameObjective<NoMovesLeft> DefaultObjective;

}       // namespace search
}       // namespace dctl
