#include <dctl/egdb/divide.hpp>
#include <dctl/board.hpp>
#include <dctl/rules.hpp>
#include <dctl/state.hpp>

using namespace dctl;

using Rules = rules::international;
using Board = board::rectangular<6,6>;
using State = state<Rules, Board>;

using egdb::dependencies;

int main()
{
        dependencies<position_t<State>> db(6,6);
        //db.print_vertices();
        //db.print_edges();
        //db.sort();
}
