#include <boost/program_options.hpp>
#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
        namespace po = boost::program_options;

        try {
                std::string const usage =
                        "Usage:                                                                          \n"
                        "       perft [-buv] <depth> [<position>]                                        \n"
                        "       perft --help                                                             \n"
                        "       perft --version                                                          \n"
                        "Performance test for search graph traversal for draughts and checkers variants. \n\n"
                ;

                std::string const synopsis =
                        "Traverses a search graph from a given starting position and counts the number of\n"
                        "successor paths at a limited depth. Only positions at remaining \"depth 0\" are \n"
                        "counted. Positions without legal moves at higher remaining depths are ignored.  \n"
                        "Draws by repetition, insufficient material or no progress are also ignored.     \n"
                        "Iteration is over depth limit. Output per iteration is the number of successor  \n"
                        "paths at the depth limit and the elapsed time for that iteration. This enables  \n"
                        "both correctness and efficiency comparisons for different legal move generators.\n\n"
                ;

                std::string const version =
                        "perft (DCTL perft) 0.1                                                          \n"
                        "Copyright (C) 2016 Rein Halbersma.                                              \n"
                        "License: Boost Software License v1.0 <http://www.boost.org/users/license.html>. \n"
                        "This is free software: you are free to change and redistribute it.              \n"
                        "There is NO WARRANTY, to the extent permitted by law.                           \n\n"

                        "Written by Rein Halbersma, see <https://bitbucket.org/rhalbersma/dctl>."
                ;

                std::string const arguments =
                        "<depth>: \n"
                        "<position>: \n"
                ;

                bool flag_bulk = false, flag_unique = false, flag_verbose = false;

                po::options_description options("Options");
                options.add_options()
                        (
                                "bulk,b",
                                po::bool_switch(&flag_bulk),
                                "Instead of counting positions at \"depth 0\", return the number of moves generated at \"depth 1\"."
                        )
                        (
                                "hash,h",
                                po::value<int>()->
                                value_name("<size>"),
                                "Use a hash table of <size> megabytes to reduce the tree."
                        )
                        (
                                "unique,u",
                                po::bool_switch(&flag_unique),
                                "Only keep unique moves during move generation."
                        )
                        (
                                "verbose,v",
                                po::bool_switch(&flag_verbose),
                                "Give debugging information."
                        )
                ;

                po::options_description operands("Operands");
                operands.add_options()
                        (
                                "depth,d",
                                po::value<int>()->
                                value_name("<depth>")->
                                required(),
                                "The depth to traverse the tree from the starting position."
                        )
                        (
                                "position,p",
                                po::value<std::string>()->
                                value_name("<position>")->
                                default_value("initial"),
                                "The position to traverse."
                        )
                ;
                po::positional_options_description positional;
                positional.add("depth", 1);
                positional.add("position", 1);

                po::options_description standard("Standard options");
                standard.add_options()
                        ("help",    "Display this help text and exit.")
                        ("version", "Display version information and exit.")
                ;

                auto visible_options = options;
                options.add(operands).add(standard);
                visible_options.add(standard);

                po::variables_map vm;
                po::store(po::command_line_parser(argc, argv)
                        .options(options)
                        .positional(positional)
                        .run(),
                        vm
                );

                if (vm.count("help")) {
                        std::cout << usage << visible_options << '\n' << arguments << '\n' << synopsis << '\n';
                        return 0;
                }
                if (vm.count("version")) {
                        std::cout << version << '\n';
                        return 0;
                }

                po::notify(vm);

                if (flag_bulk) {
                        std::cout << "bulk-counting at \"depth 1\"" << '\n';
                }
                if (vm.count("hash")) {
                        std::cout << "using hashing" << '\n';
                }
                if (flag_unique) {
                        std::cout << "unique successors" << '\n';
                }
                if (flag_verbose) {
                        std::cout << "verbose output" << '\n';
                }
                if (vm.count("depth")) {
                        std::cout << vm["depth"].as<int>() << '\n';
                }
                if (vm.count("position")) {
                        std::cout << vm["position"].as<std::string>() << '\n';
                }
                std::cout << "calling perft code, supplying parameters from <vm> variable\n";
        }
        catch(std::exception& e) {
                std::cerr << "error: " << e.what() << '\n';
                return 1;
        }
        catch(...) {
                std::cerr << "Exception of unknown type!\n";
                return 1;
        }
}
