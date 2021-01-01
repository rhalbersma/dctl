//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/egdb/divide.hpp>
#include <dctl/core.hpp>

#include <locale>

using namespace dctl::core;
using namespace dctl::egdb;

using state_type = basic_state<international, basic_board<micro>>;

int main()
{
        std::cout.imbue( std::locale( "en_US.utf8" ));
        dependencies<position_t<state_type>> db(6,6);
        //db.print_vertices();
        //db.print_edges();
        //db.sort();
}
