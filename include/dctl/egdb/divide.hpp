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

template<class Position>
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
                        divide(in);
                }
                all_edges();
        }

        auto solve(int b, int w)
        {
                auto const n = b + w;
                for (auto in = 2; in <= n; ++in) {
                        for (auto ib = std::max(1, in - w); ib <= std::min(b, in - 1); ++ib) {
                                auto const iw = in - ib;
                                assert(std::max(1, in - b) <= iw); assert(iw <= std::min(w, in - 1));
                                divide(ib, iw);
                        }
                }
                all_edges();
        }

        auto solve(int b, int w, int bp, int wp)
        {
                assert(0 <= bp); assert(bp <= b);
                assert(0 <= wp); assert(wp <= w);
                auto const n = b + w;
                for (auto in = 2; in <= n; ++in) {
                        for (auto ib = std::max(1, in - w); ib <= std::min(b, in - 1); ++ib) {
                                auto const iw = in - ib;
                                assert(std::max(1, in - b) <= iw); assert(iw <= std::min(w, in - 1));
                                for (auto ibp = 0; ibp <= std::min(bp, ib); ++ibp) {
                                        for (auto iwp = 0; iwp <= std::min(wp, iw); ++iwp) {
                                                divide(ib, iw, ibp, iwp);
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
                std::cout << "Topologically sorted = ";
                std::cout << std::boolalpha << std::is_sorted(ordered.begin(), ordered.end()) << "\n";
        }

        void print_size(subdivision const& s) const
        {
                auto const sz = index<Position>(s[0], s[1], s[2], s[3]).size();
                std::cout << "size = " << static_cast<double>(sz) / static_cast<double>(1ULL<<32) << " GiB\n";
        }

private:
        void divide(int n)
        {
                assert(2 <= n);
                for (auto b = 1; b < n; ++b) {
                        auto const w = n - b;
                        assert(1 <= w); assert(w < n);
                        divide(b, w);
                }
        }

        void divide(int b, int w)
        {
                assert(1 <= b); assert(1 <= w);
                for (auto bp = 0; bp <= b; ++bp) {
                        for (auto wp = 0; wp <= w; ++wp) {
                                divide(b, w, bp, wp);
                        }
                }
        }

        void divide(int b, int w, int bp, int wp)
        {
                assert(0 <= bp); assert(bp <= b);
                assert(0 <= wp); assert(wp <= w);
                auto const n = b + w;
                std::cout << "Adding: db" << n << "-" << b << w << "-" << bp << wp << ", ";
                m_subdivisions.push_back(subdivision{{bp, wp, b - bp, w - wp}});
                print_size(m_subdivisions.back());
        }

        void all_edges()
        {
                std::vector<index<Position>> indices;
                std::transform(m_subdivisions.begin(), m_subdivisions.end(), std::back_inserter(indices), [](auto const& s) {
                        return index<Position>(s[0], s[1], s[2], s[3]);
                });

                auto const sum = std::accumulate(indices.begin(), indices.end(), int64_t{0}, [](auto res, auto const& i) {
                        return res + i.size();
                });
                auto const max = (*std::max_element(indices.begin(), indices.end(), [](auto const& lhs, auto const& rhs) {
                        return lhs.size() < rhs.size();
                })).size();

                std::cout << "Total files = " << indices.size() << "\n";
                std::cout << "Total size = " << static_cast<double>(sum) / static_cast<double>(1ULL<<32) << " GiB\n";
                std::cout << "Max size = " << static_cast<double>(max) / static_cast<double>(1ULL<<32) << " GiB\n";

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
