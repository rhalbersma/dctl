#pragma once
#include <dctl/aima/egdb/binomial.hpp>
#include <dctl/aima/egdb/colex.hpp>
#include <dctl/aima/egdb/obstr.hpp>
#include <dctl/board_traits.hpp>
#include <dctl/color_piece.hpp>
#include <dctl/state.hpp>
#include <dctl/utility/type_traits.hpp>
#include <boost/math/special_functions/binomial.hpp>
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <iostream>

namespace dctl {
namespace egdb {

inline
auto choose(int n, int k)
{
        //if (n < k) {
        //        return int64_t{0};
        //}
        //return static_cast<int64_t>(boost::math::binomial_coefficient<double>(static_cast<unsigned>(n), static_cast<unsigned>(k)));
        return static_cast<int64_t>(BinomialTable<60,8>::coefficient(n, k));
}

template<class IntSet, class UnaryFunction>
auto reverse_colex_rank_combination(IntSet const& is, UnaryFunction fun)
{
        assert(std::is_sorted(is.begin(), is.end()));
        auto index = int64_t{0};
        auto i = 0;
        is.reverse_for_each([&](auto const sq){
                index += choose(fun(sq), ++i);
        });
        return index;
}

template<class IntSet, class UnaryFunction>
auto colex_rank_combination(IntSet const& is, UnaryFunction fun)
{
        assert(std::is_sorted(is.begin(), is.end()));
        auto index = int64_t{0};
        auto i = 0;
        is.for_each([&](auto const sq){
                index += choose(fun(sq), ++i);
        });
        return index;
}

template<class IntSet, class UnaryFunction>
auto colex_unrank_combination(int64_t index, int const N, int const K, UnaryFunction fun)
{
        IntSet is{};
        for (auto sq = N, i = K; i > 0; --sq, --i) {
                while (choose(sq, i) > index) {
                        --sq;
                }
                is.insert(fun(sq));
                index -= choose(sq, i);
        }
        assert(std::is_sorted(is.begin(), is.end()));
        return is;
}

template<class Position>
class subdatabase
{
        using board_type = board_t<Position>;
        using   set_type =   set_t<Position>;

        static constexpr auto bp_squares = (squares_v<board_type> ^ promotion_v<board_type, black_>).size();
        static constexpr auto wp_squares = (squares_v<board_type> ^ promotion_v<board_type, white_>).size();
        static constexpr auto bk_squares =  squares_v<board_type>.size();
        static constexpr auto wk_squares =  squares_v<board_type>.size();
            int bp_count, wp_count, bk_count, wk_count;
        int64_t wk_power, bk_power, wp_power, bp_power;
        int64_t capacity;
        std::vector<int> dtm;
public:
        subdatabase(int nbp, int nwp, int nbk, int nwk)
        :
                bp_count{nbp},
                wp_count{nwp},
                bk_count{nbk},
                wk_count{nwk},
                wk_power{1ULL},
                bk_power{wk_power * choose(wk_squares, wk_count)},
                wp_power{bk_power * choose(bk_squares, bk_count)},
                bp_power{wp_power * choose(wp_squares, wp_count)},
                capacity{bp_power * choose(bp_squares, bp_count)},
                dtm(static_cast<std::size_t>(capacity))
        {}

        auto init()
        {
                std::cout << "Capacity = " << capacity << "\n";
                auto const t0 = std::chrono::system_clock::now();
                int64_t num_pos = 0;
                for (int64_t index = 0; index < capacity; ++index) {
                        auto const p = unrank_position(index);
                        if (!p) { dtm[static_cast<std::size_t>(index)] = - 1; continue; }
                        num_pos += (rank_position(p.value()) == index);
                }

                using namespace std::chrono;
                auto const dt = system_clock::now() - t0;
                auto const  s = duration<double>(dt);
                auto const us = duration<double, std::micro>(dt);
                std::cout << "Legal = " << num_pos << " nodes, ";
                std::cout << s.count() << "s, ";
                std::cout << num_pos/us.count() << "Mnps" << "\n";
        }

        auto pass() const
        {
                std::cout << "Capacity = " << capacity << "\n";
                auto const t0 = std::chrono::system_clock::now();
                int64_t num_pos = 0;
                for (int64_t index = 0; index < capacity; ++index) {
                        if (dtm[static_cast<std::size_t>(index)] == -1) { continue; }
                        auto const p = unrank_position(index);
                        num_pos += (rank_position(p.value()) == index);
                }

                using namespace std::chrono;
                auto const dt = system_clock::now() - t0;
                auto const  s = duration<double>(dt);
                auto const us = duration<double, std::micro>(dt);
                std::cout << "Legal = " << num_pos << " nodes, ";
                std::cout << s.count() << "s, ";
                std::cout << num_pos/us.count() << "Mnps" << "\n";
        }

private:
        auto rank_position(Position const& p) const
        {
                auto const bp_index = reverse_colex_rank_combination(p.pieces(black_c, pawns_c), bp_ext);
                auto const wp_index =         colex_rank_combination(p.pieces(white_c, pawns_c), wp_ext);
                auto const bk_index = reverse_colex_rank_combination(p.pieces(black_c, kings_c), bk_ext);
                auto const wk_index =         colex_rank_combination(p.pieces(white_c, kings_c), wk_ext);

                auto const index = bp_index * bp_power + wp_index * wp_power + bk_index * bk_power + wk_index;
                assert(0 <= index); assert(index < capacity);
                return index;
        }

        auto unrank_position(int64_t index) const
        {
                assert(0 <= index); assert(index < capacity);

                auto const bp_index = index / bp_power; index %= bp_power;
                auto const wp_index = index / wp_power; index %= wp_power;
                auto const bk_index = index / bk_power; index %= bk_power;
                auto const wk_index = index;

                auto const bp = colex_unrank_combination<set_type>(bp_index, bp_squares, bp_count, bp_dep);
                auto const wp = colex_unrank_combination<set_type>(wp_index, wp_squares, wp_count, wp_dep);
                auto const bk = colex_unrank_combination<set_type>(bk_index, bk_squares, bk_count, bk_dep);
                auto const wk = colex_unrank_combination<set_type>(wk_index, wk_squares, wk_count, wk_dep);

                return make_position<Position>(bp, wp, bk, wk);
        }

        static auto bp_ext(int n) { return -board_type::square_from_bit(n) + bp_squares - 1;          }
        static auto wp_ext(int n) { return +board_type::square_from_bit(n) - wk_squares + wp_squares; }
        static auto bk_ext(int n) { return -board_type::square_from_bit(n) + bk_squares - 1;          }
        static auto wk_ext(int n) { return +board_type::square_from_bit(n);                           }

        static auto bp_dep(int n) { return board_type::bit_from_square(-n + bp_squares - 1         ); }
        static auto wp_dep(int n) { return board_type::bit_from_square(+n + wk_squares - wp_squares); }
        static auto bk_dep(int n) { return board_type::bit_from_square(-n + bk_squares - 1         ); }
        static auto wk_dep(int n) { return board_type::bit_from_square(+n                          ); }

        // for debugging

        auto print_set(set_type const& is) const
        {
                std::cout << " { ";
                is.for_each([&](auto const sq){
                        std::cout << (board_type::square_from_bit(sq)) << ", ";
                });
                std::cout << " } ";
        }

        auto print_pos(Position const& p) const
        {
                print_set(p.pieces(black_c, pawns_c));
                print_set(p.pieces(white_c, pawns_c));
                print_set(p.pieces(black_c, kings_c));
                print_set(p.pieces(white_c, kings_c));
        }
};

}       // namespace egdb
}       // namespace dctl
