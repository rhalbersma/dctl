#include <dctl/egdb/divide.hpp>
#include <dctl/core/board.hpp>
#include <dctl/core/rules.hpp>
#include <dctl/core/state.hpp>

#include <locale>

using namespace dctl::core;
using namespace dctl::egdb;

using Rules = international;
using Board = board::rectangular<6,6>;
using State = state<Rules, Board>;

int main()
{
        std::cout.imbue( std::locale( "en_US.utf8" ));
        dependencies<position_t<State>> db(6,6);
        //db.print_vertices();
        //db.print_edges();
        //db.sort();
}
