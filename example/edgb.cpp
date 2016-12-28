#include <dctl/aima/egdb/divide.hpp>
#include <dctl/board.hpp>
#include <dctl/rules.hpp>
#include <dctl/state.hpp>

using namespace dctl;

using Rules = rules::international;
using Board = board::international;
using State = state<Rules, Board>;

using egdb::database;

int main()
{
        database<> db;
        db.solve(2,2,0,0);
        db.print_slices();
        db.print_vertices();
        db.print_edges();
        db.sort();
}
