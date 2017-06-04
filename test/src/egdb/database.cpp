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
