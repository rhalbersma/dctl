#pragma once

//          Copyright Rein Halbersma 2010-2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/action.hpp>
#include <dctl/egdb/index.hpp>
#include <dctl/util/type_traits.hpp>

namespace dctl::egdb {

template<class State, class Successor>
class build
{
        using position_type = position_t<State>;
        using subdb_type = subdatabase<position_type>;
        subdb_type m_subdb;

        std::vector<int> btm_dtm, wtm_dtm;
        std::array<int, 3> btm_wld{};
        std::array<int, 3> wtm_wld{};
        int num_illegal{};
        int num_legal{};

        constexpr static auto illegal = -2;
        constexpr static auto unknown = -1;
        constexpr static auto win = 0;
        constexpr static auto loss = 1;
        constexpr static auto draw = 2;

public:
        explicit build(subdb_type const& subdb)
        :
                m_subdb{subdb},
                btm_dtm(size(), unknown),
                wtm_dtm(size(), unknown)
        {}

        auto init()
        {
                std::cout << "Initializing illegal positions and immediate losses.\n";
                using namespace std::chrono;
                auto const t0 = system_clock::now();

                for (std::size_t index = 0; index < size(); ++index) {
                        auto const p = unrank_position(index);
                        if (!p) {
                                btm_dtm[index] = illegal;
                                wtm_dtm[index] = illegal;
                                ++num_illegal;
                                continue;
                        }
                        num_legal += (rank_position(p.value()) == index);

                        if (!Successor{}.detect(State{p.value(), color::black})) {
                                btm_dtm[index] = 0;
                                ++btm_wld[loss];
                        }
                        if (!Successor{}.detect(State{p.value(), color::white})) {
                                wtm_dtm[index] = 0;
                                ++wtm_wld[loss];
                        }
                }

                auto const dt = system_clock::now() - t0;
                auto const  s = duration<double>(dt);
                auto const us = duration<double, std::micro>(dt);
                std::cout << "Size = " << size() << " nodes, ";
                std::cout << s.count() << "s, ";
                std::cout << size()/us.count() << "Mnps" << "\n";
                print_wld();
        }

        auto conversion()
        {
                std::cout << "Forward pass.\n";
                using namespace std::chrono;
                auto const t0 = std::chrono::system_clock::now();

                int64_t num_pos = 0;
                for (std::size_t index = 0; index < size(); ++index) {
                        if (btm_dtm[index] == illegal) { continue; }
                        auto const p = unrank_position(index);
                        num_pos += (rank_position(p.value()) == index);
                }

                auto const dt = system_clock::now() - t0;
                auto const  s = duration<double>(dt);
                auto const us = duration<double, std::micro>(dt);
                std::cout << "Size = " << size() << " nodes, ";
                std::cout << s.count() << "s, ";
                std::cout << size()/us.count() << "Mnps" << "\n";
                print_wld();
        }

        // int64_t <-> size_t adaptors

        auto size() const noexcept
        {
                return static_cast<std::size_t>(m_subdb.size());
        }

        auto rank_position(position_type const& p) const
        {
                return static_cast<std::size_t>(m_subdb.rank_position(p));
        }

        auto unrank_position(std::size_t index) const
        {
                return m_subdb.unrank_position(static_cast<index_t<subdb_type>>(index));
        }

        // for debugging

        template<class IntSet>
        auto prbit_set(IntSet const& is) const
        {
                std::cout << " { ";
                for (auto const sq : is) {
                        std::cout << (board_t<State>::square_from_bit(sq)) << ", ";
                }
                std::cout << " } ";
        }

        template<class Position>
        auto print_pos(Position const& p) const
        {
                print_set(p.pieces(black_c, pawns_c));
                print_set(p.pieces(white_c, pawns_c));
                print_set(p.pieces(black_c, kings_c));
                print_set(p.pieces(white_c, kings_c));
        }

        auto print_wld() const
        {
                std::cout << "btm WLD count: ";
                std::cout << "W = " << btm_wld[win] << ", ";
                std::cout << "L = " << btm_wld[loss] << ", ";
                std::cout << "D = " << btm_wld[draw] << "\n";
                std::cout << "wtm WLD count: ";
                std::cout << "W = " << wtm_wld[win] << ", ";
                std::cout << "L = " << wtm_wld[loss] << ", ";
                std::cout << "D = " << wtm_wld[draw] << "\n";
        }

};

}       // namespace dctl::egdb
