#include <dctl/egdb/divide.hpp>
#include <dctl/board.hpp>
#include <dctl/rules.hpp>
#include <dctl/state.hpp>

#include <locale>

using namespace dctl;

using Rules = rules::international;
using Board = board::rectangular<8,8>;
using State = state<Rules, Board>;

using egdb::dependencies;


int main()
{
        std::cout.imbue( std::locale( "en_US.utf8" ));
        dependencies<position_t<State>> db(6,6);
        //db.print_vertices();
        //db.print_edges();
        //db.sort();
}
