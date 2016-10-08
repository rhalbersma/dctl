#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
        namespace po = boost::program_options;

        try {
                po::options_description desc(
                        "NAME                                                                            \n\t"
                                "perft, divide - performance test for search tree traversal              \n\n\t"

                        "SYNOPSIS                                                                        \n"
                                "perft [OPTIONS]... POSITION                                             \n\n\t"

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
                desc.add_options()
                        ("help", "\nproduce help message")
                        ("depth,d"  , po::value<int>()                       , "\nThe depth to traverse the tree from the starting position.")
                        ("FEN,F"    , po::value<std::string>()               , "\nThe position to traverse.")
                        ("unique,u" , po::bool_switch()->default_value(false), "\nOnly keep unique moves during move generation.")
                        ("verbose,v", po::bool_switch()->default_value(false), "\nGive debugging information.")
                        ("bulk,b"   , po::bool_switch()->default_value(false), "\nInstead of counting nodes at \"depth 0\", return the number of moves generated at \"depth 1\".")
                        ("hash,h"   , po::bool_switch()->default_value(false), "\nUse hashing to reduce the tree size.")
                ;

                po::positional_options_description p;
                p.add("FEN", 1);

                po::variables_map vm;
                po::store(po::command_line_parser(argc, argv).
                          options(desc).positional(p).run(), vm);
                po::notify(vm);

                if (vm.count("help")) {
                        std::cout << desc << "\n";
                        return 0;
                }

                if (vm.count("depth")) {
                        std::cout << "Depth level was set to "
                         << vm["depth"].as<int>() << ".\n";
                } else {
                        std::cout << argv[0] << ": missing depth operand.\n";
                        std::cout << "Try '"<< argv[0] << " --help' for more information.\n";
                }

                if (vm.count("FEN")) {
                        std::cout << "Position to traverse is "
                         << vm["FEN"].as<std::string>() << ".\n";
                } else {
                        std::cout << argv[0] << ": missing position operand.\n";
                        std::cout << "Try '"<< argv[0] << " --help' for more information.\n";
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
