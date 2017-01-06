#include <dctl/egdb/divide.hpp>
#include <dctl/board.hpp>
#include <dctl/rules.hpp>
#include <dctl/state.hpp>

using namespace dctl;

using Rules = rules::international;
using Board = board::rectangular<10,10>;
using State = state<Rules, Board>;

using egdb::dbx;

int main()
{
        dbx<position_t<State>> db;
        db.solve(3,3);
        //db.print_slices();
        //db.print_vertices();
        //db.print_edges();
        db.sort();
}
