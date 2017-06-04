#pragma once
#include <dctl/core.hpp>
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

template<class Position>
class dependencies
{
        using subdivision_type = subdivision<Position>;
        std::vector<subdivision_type> m_subdivisions;
        using Graph = boost::adjacency_list<>;
        Graph m_graph;
        using edge_type = std::pair<int, int>;
        using vertex_map = boost::property_map<Graph, boost::vertex_index_t>::type ;
        vertex_map m_vertex_map;
public:
        dependencies(int n)
        {
                for (auto in = 2; in <= n; ++in) {
                        subdivide(in);
                }
                create_graph();
        }

        dependencies(int b, int w)
        {
                auto const n = b + w;
                for (auto in = 2; in <= n; ++in) {
                        for (auto ib = std::max(1, in - w); ib <= std::min(b, in - 1); ++ib) {
                                auto const iw = in - ib;
                                assert(std::max(1, in - b) <= iw); assert(iw <= std::min(w, in - 1));
                                subdivide(ib, iw);
                        }
                }
                create_graph();
        }

        dependencies(int b, int w, int bp, int wp)
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
                                                subdivide(ib, iw, ibp, iwp);
                                        }
                                }
                        }
                }
                create_graph();
        }

        auto print_vertices() const
        {
                std::cout << "---------\n";

                std::cout << "vertices(g) = " << boost::num_vertices(m_graph) << " = ";
                for (auto vp = vertices(m_graph); vp.first != vp.second; ++vp.first) {
                        std::cout << m_vertex_map[*vp.first] <<  " ";
                }
                std::cout << "\n";
        }

        auto print_edges() const
        {
                std::cout << "---------\n";

                std::cout << "edges(g) = " << boost::num_edges(m_graph) << " = ";
                boost::graph_traits<Graph>::edge_iterator ei, ei_end;
                for (boost::tie(ei, ei_end) = boost::edges(m_graph); ei != ei_end; ++ei) {
                        std::cout << "(" << m_vertex_map[boost::source(*ei, m_graph)]
                                  << "," << m_vertex_map[boost::target(*ei, m_graph)] << ") ";
                }
                std::cout << "\n";
        }

        auto sort()
        {
                std::vector<unsigned> ordered;
                boost::topological_sort(m_graph, std::back_inserter(ordered));
                std::cout << "Topologically sorted = ";
                std::cout << std::boolalpha << std::is_sorted(ordered.begin(), ordered.end()) << "\n";
        }
private:
        void subdivide(int n)
        {
                assert(2 <= n);
                for (auto b = 1; b < n; ++b) {
                        auto const w = n - b;
                        assert(1 <= w); assert(w < n);
                        subdivide(b, w);
                }
        }

        void subdivide(int b, int w)
        {
                assert(1 <= b); assert(1 <= w);
                for (auto bp = 0; bp <= b; ++bp) {
                        for (auto wp = 0; wp <= w; ++wp) {
                                subdivide(b, w, bp, wp);
                        }
                }
        }

        void subdivide(int b, int w, int bp, int wp)
        {
                assert(0 <= bp); assert(bp <= b);
                assert(0 <= wp); assert(wp <= w);
                m_subdivisions.emplace_back(b, w, bp, wp);
                auto const& s = m_subdivisions.back();
                std::cout << "Adding: " << s.to_string() << ", ";
                std::cout << "size = " << static_cast<double>(s.size()) / static_cast<double>(1ULL<<32) << " GiB, ";
                std::cout << "legal = " << s.legal() << "\n";
        }

        void create_graph()
        {
                auto const sum = std::accumulate(m_subdivisions.begin(), m_subdivisions.end(), int64_t{0}, [](auto res, auto const& s) {
                        return res + s.size();
                });

                auto const leg = std::accumulate(m_subdivisions.begin(), m_subdivisions.end(), int64_t{0}, [](auto res, auto const& s) {
                        return res + s.legal();
                });

                auto const max = (*std::max_element(m_subdivisions.begin(), m_subdivisions.end(), [](auto const& lhs, auto const& rhs) {
                        return lhs.size() < rhs.size();
                })).size();

                std::sort(m_subdivisions.begin(), m_subdivisions.end(), [](auto const& lhs, auto const& rhs){
                        return lhs.size() < rhs.size();
                });

                for (auto const& s : m_subdivisions) {
                        std::cout << s.to_string() << ", size = ";
                        std::cout << s.size() << ", ";
                        std::cout << "legal = " << s.legal() << ", ";
                        std::cout << "overhead = " << static_cast<double>(s.size()) / static_cast<double>(s.legal()) << "\n";
                }

                std::cout << "Total files = " << m_subdivisions.size() << "\n";
                std::cout << "Total size = " << sum << "\n";
                std::cout << "Total positions = " << leg << "\n";
                std::cout << "Max size = " << static_cast<double>(max) / static_cast<double>(1ULL<<32) << " GiB\n";
/*
                for (auto i = 0u; i < m_subdivisions.size(); ++i) {
                        for (auto j = i + 1; j < m_subdivisions.size(); ++j) {
                                if (depends_on{}(m_subdivisions[j], m_subdivisions[i])) {
                                        boost::add_edge(j, i, m_graph);
                                }
                                assert(!depends_on{}(m_subdivisions[i], m_subdivisions[j]));
                        }
                }
                m_vertex_map = boost::get(boost::vertex_index, m_graph);*/
        }

};

}       // namespace egdb
}       // namespace dctl
