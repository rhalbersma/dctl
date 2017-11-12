//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/algo/node.hpp>
#include <dctl/core.hpp>
#include <chrono>               // duration_cast, milliseconds, steady_clock
#include <iomanip>              // setw
#include <iostream>             // cout
#include <list>                 // list
#include <locale>               // locale
#include <map>                  // map
#include <numeric>              // accumulate
#include <utility>              // pair
#include <vector>               // vector

using namespace dctl::core;
using namespace dctl::algo;

using rules_type = checkers;
using board_type = basic_board<checkers>;
using state_type = basic_state<rules_type, board_type>;

using count_type = int64_t;
using cache = std::map<state_type, count_type>;
using layer = std::list<cache>;
using trans = std::map<std::pair<state_type, std::size_t>, count_type>;

auto const successor = keep_duplicates_gen;
auto const memory_bound = std::size_t{16};
auto const depth = std::size_t{28};

using clock_type = std::chrono::steady_clock;

inline auto perft(state_type const& s, std::size_t d)
{
        if (d == 1) {
                return static_cast<count_type>(successor.count(s));
        }

        auto const moves = successor.generate(s);
        return std::accumulate(moves.begin(), moves.end(), count_type{0}, [&](auto const n, auto const& m) {
                return n + perft(result(s, m), d - 1);
        });
}

inline auto perft(state_type const& s, std::size_t d, trans& tt)
{
        if (auto entry = tt.find({s, d}); entry != tt.end()) {
                return entry->second;
        }

        count_type nodes;
        if (d == 1) {
                nodes = static_cast<count_type>(successor.count(s));
        } else {
                auto const moves = successor.generate(s);
                nodes = std::accumulate(moves.begin(), moves.end(), count_type{0}, [&](auto const n, auto const& m) {
                        return n + perft(result(s, m), d - 1, tt);
                });
        }

        tt.insert({{s, d}, nodes});
        return nodes;
}

int main()
{
        std::cout.imbue(std::locale("en_US.utf8"));

        auto frontier = std::vector<std::pair<state_type, count_type>>{};
        {
                auto N = std::vector<count_type>{1};
                auto U = layer{{{state_type::initial(), 1}}};

                for (auto d = decltype(depth){1}; d <= memory_bound; ++d) {
                        auto const t0 = clock_type::now();
                        N.push_back(0);
                        U.push_back({});
                        for (auto const& [s, n] : U.front()) {
                                auto const moves = successor.generate(s);
                                for (auto const& m : moves) {
                                        N[d] += n;
                                        U.back()[result(s, m)] += n;
                                }
                        }
                        U.pop_front();
                        auto const t1 = clock_type::now();

                        std::cout << "info ";
                        std::cout << "depth " << std::setw(2) << d << " ";
                        std::cout << "nodes " << std::setw(21) << N[d] << " ";
                        std::cout << "time " << std::setw(10) << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << " ms ";
                        std::cout << "unique " << std::setw(20) << U.front().size() << "\n";
                }

                std::cout << "copy std::map to std::vector, ";
                auto const t0 = clock_type::now();
                frontier.assign(U.front().begin(), U.front().end());
                auto const t1 = clock_type::now();
                std::cout << "completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << " ms\n";
        }

        for (auto d = decltype(depth){1}; d <= depth - memory_bound; ++d) {
                auto const t0 = clock_type::now();
                auto const nodes = std::accumulate(frontier.begin(), frontier.end(), count_type{0}, [&](auto const result, auto const& element) {
                        return result + element.second * perft(element.first, d);
                });
                auto const t1 = clock_type::now();

                std::cout << "info ";
                std::cout << "depth " << std::setw(2) << (memory_bound + d) << " ";
                std::cout << "nodes " << std::setw(21) << nodes << " ";
                std::cout << "time " << std::setw(10) << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << " ms\n";
        }
}
