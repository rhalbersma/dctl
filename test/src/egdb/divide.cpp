#include <dctl/egdb/divide.hpp>
#include <dctl/board.hpp>
#include <dctl/rules.hpp>
#include <dctl/state.hpp>

using namespace dctl;

using Rules = rules::international;
using Board = board::rectangular<6,6>;
using State = state<Rules, Board>;

using egdb::dbx;

int main()
{
        dbx<position_t<state<rules::international, board::rectangular<6,6>>>> db;
        db.solve(6);
        db.print_size();
        //db.print_slices();
        //db.print_vertices();
        //db.print_edges();
        db.sort();
}
