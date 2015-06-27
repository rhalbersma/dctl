#include <search/fixture.hpp>
#include <dctl/board/types.hpp>
#include <dctl/rules.hpp>
#include <dctl/setup/string.hpp>

using namespace dctl;

using Rules = rules::International;
using Board = board::International;
using F = search::Fixture< State<Rules, Board>, search::DefaultObjective >;

int main()
{
        auto const p = State<Rules, Board>::initial();
        F f;
        f.root_.analyze(p, search::unique_gen, 71);
}
