//          Copyright Rein Halbersma 2010-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/egdb/database.hpp>       // database
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_CHECK_EQUAL, BOOST_AUTO_TEST_SUITE_END

using namespace dctl::egdb;

BOOST_AUTO_TEST_SUITE(Database)

BOOST_AUTO_TEST_CASE(ScanDirPath)
{
        database db("/media/sf_C_DRIVE/Program Files/Kingsrow International/wld_database");
        db.scan_directory();    // print out all Kingsrow dbs and their combined size on disk
}

BOOST_AUTO_TEST_SUITE_END()
