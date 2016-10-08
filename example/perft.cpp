#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
        namespace po = boost::program_options;

        try {
                po::options_description man(
                        "NAME                                                                            \n\t"
                                "perft, divide - performance test for search tree traversal              \n\n"

                        "SYNOPSIS                                                                        \n\t"
                                "perft [OPTIONS] <POSITION>                                              \n\n"

                        "DESCRIPTION                                                                     \n\t"
                                "perft traverses a search tree from a given position to a limited depth. \n\t"
                                "Only nodes at depth = 0 are counted; positions without legal moves at   \n\t"
                                "higher depths are ignored. perft also ignores draws by repetition, no   \n\t"
                                "progress or insufficient material. perft iterates over depth, and by    \n\t"
                                "recording the time for each iteration, the performance of different     \n\t"
                                "move generators can be compared.                                        \n\n\t"

                                "In addition, the variant program divide lists all legal moves in a given\n\t"
                                "position, and, for each move, calls perft with parameter depth - 1      \n\n"

                        "OPTIONS"
                );

                po::options_description gen("General options");
                gen.add_options()
                        ("help", "\nOutput a usage message and exit.")
                        ("depth,d"   , po::value<int>()        ->value_name("DEPTH")   , "\nThe depth to traverse the tree from the starting position.")
                        ("position,p", po::value<std::string>()->value_name("[format_options] POSITION"), "\nThe position to traverse.")
                        ("unique,u"  , po::bool_switch()->default_value(false), "\nOnly keep unique moves during move generation.")
                        ("verbose,v" , po::bool_switch()->default_value(false), "\nGive debugging information.")
                ;

                po::options_description opt("Optimization options");
                opt.add_options()
                        ("bulk,b", po::bool_switch()->default_value(false), "\nInstead of counting nodes at \"depth 0\", return the number of moves generated at \"depth 1\".")
                        ("hash,h", po::value<int>()->value_name("SIZE"), "\nUse a hash table of SIZE megabytes to reduce the tree.")
                ;

                man.add(gen).add(opt);

                po::positional_options_description p;
                p.add("position", 1);

                po::variables_map vm;
                po::store(po::command_line_parser(argc, argv)
                        .options(man)
                        .positional(p)
                        .run(),
                        vm
                );
                po::notify(vm);

                if (vm.count("help")) {
                        std::cout << man << "\n";
                        return 0;
                }
        }

        catch(std::exception& e) {
                std::cerr << "error: " << e.what() << "\n";
                return 1;
        }
        catch(...) {
                std::cerr << "Exception of unknown type!\n";
        }

        /* call perft code here, supplying parameters from <vm> variable */
}
