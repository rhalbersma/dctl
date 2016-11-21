#include <dctl/action.hpp>
#include <dctl/actions.hpp>
#include <dctl/aima/node.hpp>
#include <dctl/board_traits.hpp>
#include <dctl/board/mask/squares.hpp>
#include <dctl/color_piece.hpp>
#include <dctl/setup/string.hpp>
#include <dctl/state.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>     // any_of
#include <boost/algorithm/cxx11/none_of.hpp>    // none_of
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/range/combine.hpp>              // combine
#include <boost/tuple/tuple.hpp>                // get
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>

#include <dctl/rules.hpp>
#include <dctl/board.hpp>

using namespace dctl;

using Rules = rules::international;
using Board = board::international;
using State = state<Rules, Board>;

using Slice = std::array<int, 4>;

template<class T, std::size_t N>
auto xprint(std::array<T, N> const& m)
{
        std::copy(m.begin(), m.end(), std::ostream_iterator<T>(std::cout, ","));
}

template<class State>
auto get_slice(State const& s)
        -> Slice
{
        return {{
                static_cast<int>(s.num_pieces(black_c, pawns_c)),
                static_cast<int>(s.num_pieces(black_c, kings_c)),
                static_cast<int>(s.num_pieces(white_c, pawns_c)),
                static_cast<int>(s.num_pieces(white_c, kings_c))
        }};
}

class depends_on
{
        auto depends_on_black_jump(Slice const& lhs, Slice const& rhs) const
        {
                return
                        (
                                (lhs[0]     == rhs[0] && lhs[1]     == rhs[1]) ||
                                (lhs[0] - 1 == rhs[0] && lhs[1] + 1 == rhs[1])
                        ) &&
                        (
                                (lhs[2]     >  rhs[2] && lhs[3]     >= rhs[3]) ||
                                (lhs[2]     >= rhs[2] && lhs[3]     >  rhs[3])
                        )
                ;
        }

        auto depends_on_white_jump(Slice const& lhs, Slice const& rhs) const
        {
                return
                        (
                                (lhs[2]     == rhs[2] && lhs[3]     == rhs[3]) ||
                                (lhs[2] - 1 == rhs[2] && lhs[3] + 1 == rhs[3])
                        ) &&
                        (
                                (lhs[0]     >  rhs[0] && lhs[1]     >= rhs[1]) ||
                                (lhs[0]     >= rhs[0] && lhs[1]     >  rhs[1])
                        )
                ;
        }

        auto depends_on_black_promotion(Slice const& lhs, Slice const& rhs) const
        {
                return
                        (lhs[0] - 1 == rhs[0] && lhs[1] + 1 == rhs[1]) &&
                        (lhs[2]     == rhs[2] && lhs[3]     == rhs[3])
                ;
        }

        auto depends_on_white_promotion(Slice const& lhs, Slice const& rhs) const
        {
                return
                        (lhs[2] - 1 == rhs[2] && lhs[3] + 1 == rhs[3]) &&
                        (lhs[0]     == rhs[0] && lhs[1]     == rhs[1])
                ;
        }

public:
        auto operator()(Slice const& lhs, Slice const& rhs) const
        {
                return
                        depends_on_black_jump(lhs, rhs) ||
                        depends_on_white_jump(lhs, rhs) ||
                        depends_on_black_promotion(lhs, rhs) ||
                        depends_on_white_promotion(lhs, rhs)
                ;
        }
};
/*
class state_range
{
        Slice m_slice;
        std::array<std::vector<int>, 4> m_pieces, m_finish;
        std::array<std::vector<int>, 4> m_sentinel{};
public:
        state_range(Slice const& s)
        :
                m_slice(s)
        {
                auto const king_squares = squares_v<Board>;
                auto const bp_squares = king_squares ^ promotion_v<Board, black_>;
                auto const wp_squares = king_squares ^ promotion_v<Board, white_>;

                auto const bp_first = Board::square_from_bit(*bp_squares. begin());
                auto const bp_last  = Board::square_from_bit(*bp_squares.rbegin());
                auto const wp_first = Board::square_from_bit(*wp_squares.rbegin());
                auto const wp_last  = Board::square_from_bit(*wp_squares. begin());

                auto const bk_first = Board::square_from_bit(*king_squares. begin());
                auto const bk_last  = Board::square_from_bit(*king_squares.rbegin());
                auto const wk_first = Board::square_from_bit(*king_squares.rbegin());
                auto const wk_last  = Board::square_from_bit(*king_squares. begin());

                assert(bp_first == bk_first);
                assert(wp_first == wk_first);

                for (auto i = 0u; i < 4; ++i) {
                        m_pieces[i].reserve(m_slice[i]);
                        m_finish[i].reserve(m_slice[i]);
                }

                for (auto i = 0u; i < m_slice[0]; ++i) {
                        m_pieces[0].push_back(bp_first + i);
                        m_finish[0].push_back(bp_last  - i);
                }

                for (auto i = 0u; i < m_slice[2]; ++i) {
                        m_pieces[2].push_back(wp_first - i);
                        m_finish[2].push_back(wp_last  + i);
                }

                for (auto i = 0u; i < m_slice[1]; ++i) {
                        m_pieces[1].push_back(bk_first + m_slice[0] + i);
                        m_finish[1].push_back(bk_last               - i);
                }

                for (auto i = 0u; i < m_slice[3]; ++i) {
                        m_pieces[3].push_back(wk_first - m_slice[0] - i);
                        m_finish[3].push_back(wk_last               + i);
                }
        }

        auto pop_front()
        {
                if (m_pieces == m_finish) {
                        m_pieces = m_sentinel;
                }
        }

        auto front() const
        {
                std::array<set_t<Board>, 4> bb{};
                for (auto i = 0u; i < 4; ++i) {
                        for (auto const& sq : m_pieces[i]) {
                                bb[i].set(Board::bit_from_square(sq));
                        }
                }
                return position_t<State>{bb[0], bb[1], bb[2], bb[3]};
        }

        auto back() const
        {
                std::array<set_t<Board>, 4> bb{};
                for (auto i = 0u; i < 4; ++i) {
                        for (auto const& sq : m_finish[i]) {
                                bb[i].set(Board::bit_from_square(sq));
                        }
                }
                return position_t<State>{bb[0], bb[1], bb[2], bb[3]};
        }

        auto empty() const
        {
                return m_pieces == m_sentinel;
        }
};
*/
template<int = 0>
class database
{
        std::vector<Slice> m_slices;
        using Graph = boost::adjacency_list<>;
        Graph m_graph;
        using edge_type = std::pair<int, int>;
        using IndexMap = boost::property_map<Graph, boost::vertex_index_t>::type ;
        IndexMap m_index;
public:
        auto solve(int n)
        {
                for (auto i = 2; i <= n; ++i) {
                        divide(i);
                }
                all_edges();
        }

        auto solve(int b, int w)
        {
                for (auto ib = 1; ib <= b; ++ib) {
                        for (auto iw = 1; iw <= w; ++iw) {
                                divide(ib, iw);
                        }
                }
                all_edges();
        }

        auto solve(int bp, int bk, int wp, int wk)
        {
                for (auto ibp = 0; ibp <= bp; ++ibp) {
                        for (auto ibk = std::max(0, 1 - ibp); ibk <= bk + bp - ibp; ++ibk) {
                                for (auto iwp = 0; iwp <= wp; ++iwp) {
                                        for (auto iwk = std::max(0, 1 - iwp); iwk <= wk + wp - iwp; ++iwk) {
                                                divide(ibp, ibk, iwp, iwk);
                                        }
                                }
                        }
                }
                all_edges();
        }

        auto print_slices() const
        {
                std::cout << "---------\n";
                for (auto const& s : m_slices) {
                        xprint(s); std::cout << "\n";
                }
                std::cout << "Database with " << m_slices.size() << " slices\n";
        }

        auto print_vertices() const
        {
                std::cout << "---------\n";

                std::cout << "vertices(g) = " << boost::num_vertices(m_graph) << " = ";
                for (auto vp = vertices(m_graph); vp.first != vp.second; ++vp.first)
                        std::cout << m_index[*vp.first] <<  " ";
                std::cout << "\n";
        }

        auto print_edges() const
        {
                std::cout << "---------\n";

                std::cout << "edges(g) = " << boost::num_edges(m_graph) << " = ";
                boost::graph_traits<Graph>::edge_iterator ei, ei_end;
                for (boost::tie(ei, ei_end) = boost::edges(m_graph); ei != ei_end; ++ei)
                        std::cout << "(" << m_index[boost::source(*ei, m_graph)]
                                  << "," << m_index[boost::target(*ei, m_graph)] << ") ";
                std::cout << "\n";
        }

        auto sort()
        {
                std::vector<unsigned> ordered;
                boost::topological_sort(m_graph, std::back_inserter(ordered));
                std::cout << "Sorted vertices = ";
                for (auto e : ordered)
                        std::cout << e << ", ";
                std::cout << "\n";
                std::cout << "Sorted slices = \n";
                for (auto e : ordered) {
                        xprint(m_slices[e]); std::cout << "\n";
                }
                std::cout << "\n";
        }

private:
        auto divide(int i)
        {
                assert(2 <= i);
                for (auto b = 1; b < i; ++b) {
                        divide(b, i - b);
                }
        }

        auto divide(int b, int w)
        {
                assert(1 <= b); assert(1 <= w);
                for (auto bp = 0; bp <= b; ++bp) {
                        for (auto wp = 0; wp <= w; ++wp) {
                                divide(bp, b - bp, wp, w - wp);
                        }
                }
        }

        auto divide(int bp, int bk, int wp, int wk)
        {
                assert(1 <= bp + bk); assert(1 <= wp + wk);
                m_slices.push_back(Slice{{bp, bk, wp, wk}});
        }

        auto all_edges()
        {
                for (auto i = 0u; i < m_slices.size(); ++i) {
                        for (auto j = i + 1; j < m_slices.size(); ++j) {
                                if (depends_on{}(m_slices[j], m_slices[i])) {
                                        boost::add_edge(j, i, m_graph);
                                }
                                assert(!depends_on{}(m_slices[i], m_slices[j]));
                        }
                }
                m_index = boost::get(boost::vertex_index, m_graph);
        }

};

int main()
{
        database<> db;
        db.solve(2,0,2,0);
        db.print_slices();
        db.print_vertices();
        db.print_edges();
        db.sort();

        //auto rng = state_range{Slice{{2,2,2,2}}};
        //auto s = State{rng.back(), color::black};
        //std::cout << setup::write<pdn::protocol>()(s) << "\n";
}
