#pragma once
#include <dctl/board.hpp>
#include <dctl/rules.hpp>
#include <dctl/state.hpp>
#include <dctl/board_traits.hpp>
#include <dctl/egdb/index.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>

namespace dctl {
namespace egdb {

using subdivision = std::array<int, 4>;

template<class T, std::size_t N>
auto xprint(std::array<T, N> const& m)
{
        std::copy(m.begin(), m.end(), std::ostream_iterator<T>(std::cout, ","));
}
/*
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
*/
class depends_on
{
        auto depends_on_black_jump(subdivision const& lhs, subdivision const& rhs) const
        {
                return
                        (
                                (lhs[0]     == rhs[0] && lhs[2]     == rhs[2]) ||
                                (lhs[0] - 1 == rhs[0] && lhs[2] + 1 == rhs[2])
                        ) &&
                        (
                                (lhs[1]     >  rhs[1] && lhs[3]     >= rhs[3]) ||
                                (lhs[1]     >= rhs[1] && lhs[3]     >  rhs[3])
                        )
                ;
        }

        auto depends_on_white_jump(subdivision const& lhs, subdivision const& rhs) const
        {
                return
                        (
                                (lhs[1]     == rhs[1] && lhs[3]     == rhs[3]) ||
                                (lhs[1] - 1 == rhs[1] && lhs[3] + 1 == rhs[3])
                        ) &&
                        (
                                (lhs[0]     >  rhs[0] && lhs[2]     >= rhs[2]) ||
                                (lhs[0]     >= rhs[0] && lhs[2]     >  rhs[2])
                        )
                ;
        }

        auto depends_on_black_promotion(subdivision const& lhs, subdivision const& rhs) const
        {
                return
                        (lhs[0] - 1 == rhs[0] && lhs[2] + 1 == rhs[2]) &&
                        (lhs[1]     == rhs[1] && lhs[3]     == rhs[3])
                ;
        }

        auto depends_on_white_promotion(subdivision const& lhs, subdivision const& rhs) const
        {
                return
                        (lhs[1] - 1 == rhs[1] && lhs[3] + 1 == rhs[3]) &&
                        (lhs[0]     == rhs[0] && lhs[2]     == rhs[2])
                ;
        }

public:
        auto operator()(subdivision const& lhs, subdivision const& rhs) const
        {
                return
                        depends_on_black_jump(lhs, rhs) ||
                        depends_on_white_jump(lhs, rhs) ||
                        depends_on_black_promotion(lhs, rhs) ||
                        depends_on_white_promotion(lhs, rhs)
                ;
        }
};

class dbx
{
        std::vector<subdivision> m_subdivisions;
        using Graph = boost::adjacency_list<>;
        Graph m_graph;
        using edge_type = std::pair<int, int>;
        using vertex_map = boost::property_map<Graph, boost::vertex_index_t>::type ;
        vertex_map m_vertex_map;
public:
        auto solve(int n)
        {
                for (auto in = 2; in <= n; ++in) {
                        std::cout << "level 1: " << in << "\n";
                        divide(in);
                }
                all_edges();
        }

        auto solve(int b, int w)
        {
                auto const n = b + w;
                for (auto in = 2; in <= n; ++in) {
                        std::cout << "level 1: " << in << "\n";
                        for (auto ib = std::max(1, in - w); ib <= std::min(b, in - 1); ++ib) {
                                auto const iw = in - ib;
                                assert(std::max(1, in - b) <= iw); assert(iw <= std::min(w, in - 1));
                                std::cout << "level 2: " << ib << ", " << iw << "\n";
                                divide(ib, iw);
                        }
                }
                all_edges();
        }

        auto solve(int bp, int wp, int bk, int wk)
        {
                auto const n = bp + wp + bk + wk;
                auto const b = bp + bk;
                [[maybe_unused]] auto const w = wp + wk;
                for (auto in = 2; in <= n; ++in) {
                        std::cout << "level 1: " << in << "\n";
                        for (auto ib = std::max(1, in - w); ib <= std::min(b, in - 1); ++ib) {
                                auto const iw = in - ib;
                                assert(std::max(1, in - b) <= iw); assert(iw <= std::min(w, in - 1));
                                std::cout << "level 2: " << ib << ", " << iw << "\n";
                                for (auto ibp = 0; ibp <= std::min(bp, ib); ++ibp) {
                                        auto const ibk = ib - ibp;
                                        for (auto iwp = 0; iwp <= std::min(wp, iw); ++iwp) {
                                                auto const iwk = iw - iwp;
                                                divide(ibp, iwp, ibk, iwk);
                                        }
                                }
                        }
                }
                all_edges();
        }

        auto print_slices() const
        {
                std::cout << "---------\n";
                for (auto const& s : m_subdivisions) {
                        xprint(s); std::cout << "\n";
                }
                std::cout << "Database with " << m_subdivisions.size() << " subdivisions\n";
        }

        auto print_vertices() const
        {
                std::cout << "---------\n";

                std::cout << "vertices(g) = " << boost::num_vertices(m_graph) << " = ";
                for (auto vp = vertices(m_graph); vp.first != vp.second; ++vp.first)
                        std::cout << m_vertex_map[*vp.first] <<  " ";
                std::cout << "\n";
        }

        auto print_edges() const
        {
                std::cout << "---------\n";

                std::cout << "edges(g) = " << boost::num_edges(m_graph) << " = ";
                boost::graph_traits<Graph>::edge_iterator ei, ei_end;
                for (boost::tie(ei, ei_end) = boost::edges(m_graph); ei != ei_end; ++ei)
                        std::cout << "(" << m_vertex_map[boost::source(*ei, m_graph)]
                                  << "," << m_vertex_map[boost::target(*ei, m_graph)] << ") ";
                std::cout << "\n";
        }

        auto sort()
        {
                std::vector<unsigned> ordered;
                boost::topological_sort(m_graph, std::back_inserter(ordered));
                std::cout << "Topologically sorted vertices = ";
                for (auto e : ordered)
                        std::cout << e << ", ";
                std::cout << "\n";
                std::cout << "Ordered subdivisions = \n";
                for (auto e : ordered) {
                        xprint(m_subdivisions[e]); std::cout << "\n";
                }
                std::cout << "\n";
        }

        auto print_size()
        {
                auto sum = 0L;
                auto max = 0L;
                for (auto const& s : m_subdivisions) {
                        xprint(s);
                        auto const sz = index<position_t<state<rules::international, board::rectangular<6,6>>>>(s[0], s[1], s[2], s[3]).size();
                        sum += sz;
                        max = std::max(max, sz);
                        std::cout << ", size = " << static_cast<double>(sz) / static_cast<double>(1ULL<<32) << " GiB\n";
                }
                std::cout << "Total size = " << static_cast<double>(sum) / static_cast<double>(1ULL<<32) << " GiB\n";
                std::cout << "Max size = " << static_cast<double>(max) / static_cast<double>(1ULL<<32) << " GiB\n";
        }

private:
        void divide(int n)
        {
                assert(2 <= n);
                for (auto ib = 1; ib < n; ++ib) {
                        auto const iw = n - ib;
                        assert(1 <= iw); assert(iw < n);
                        std::cout << "level 2: " << ib << ", " << iw << "\n";
                        divide(ib, iw);
                }
        }

        void divide(int b, int w)
        {
                assert(1 <= b); assert(1 <= w);
                for (auto bp = 0; bp <= b; ++bp) {
                        auto const bk = b - bp;
                        for (auto wp = 0; wp <= w; ++wp) {
                                auto const wk = w - wp;
                                divide(bp, wp, bk, wk);
                        }
                }
        }

        void divide(int bp, int wp, int bk, int wk)
        {
                assert(1 <= bp + bk); assert(1 <= wp + wk);
                std::cout << "Adding : " << bp << wp << bk << wk << "\n";
                m_subdivisions.push_back(subdivision{{bp, wp, bk, wk}});
        }

        void all_edges()
        {
                for (auto i = 0u; i < m_subdivisions.size(); ++i) {
                        for (auto j = i + 1; j < m_subdivisions.size(); ++j) {
                                if (depends_on{}(m_subdivisions[j], m_subdivisions[i])) {
                                        boost::add_edge(j, i, m_graph);
                                }
                                assert(!depends_on{}(m_subdivisions[i], m_subdivisions[j]));
                        }
                }
                m_vertex_map = boost::get(boost::vertex_index, m_graph);
        }
};

}       // namespace egdb
}       // namespace dctl
