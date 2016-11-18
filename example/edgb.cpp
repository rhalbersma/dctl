#include <dctl/action.hpp>
#include <dctl/actions.hpp>
#include <dctl/aima/node.hpp>
#include <dctl/board/mask/promotion.hpp>
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

using material = std::array<int, 4>;
using delta = std::array<int, 4>;

template<class T, std::size_t N>
auto xprint(std::array<T, N> const& m)
{
        std::copy(m.begin(), m.end(), std::ostream_iterator<T>(std::cout, ","));
}

template<class State>
auto get_material(State const& s)
        -> material
{
        return {{
                static_cast<int>(s.num_pieces(black_c, pawns_c)),
                static_cast<int>(s.num_pieces(black_c, kings_c)),
                static_cast<int>(s.num_pieces(white_c, pawns_c)),
                static_cast<int>(s.num_pieces(white_c, kings_c))
        }};
}

static auto levels(material const& m)
{
        return delta{{
                m[0] + m[1],
                m[2] + m[3],
                m[0],
                m[2]
        }};
}

struct depends_on
{
        auto operator()(material const& lhs, material const& rhs) const
        {
                auto const L = levels(lhs);
                auto const R = levels(rhs);
                return
                        boost::algorithm:: any_of(boost::combine(L, R), [](auto z) { return boost::get<0>(z) > boost::get<1>(z); }) &&
                        boost::algorithm::none_of(boost::combine(L, R), [](auto z) { return boost::get<0>(z) < boost::get<1>(z); })
                ;
        }
};

template<int = 0>
class database
{
        std::vector<material> m_slices;
        using Graph = boost::adjacency_list<>;
        Graph m_graph;
        using edge_type = std::pair<int, int>;
        using IndexMap = boost::property_map<Graph, boost::vertex_index_t>::type ;
        IndexMap m_index;
public:
        auto solve(int n)
        {
                enumerate_max(n);
                all_edges();

        }

        template<class State>
        auto find(State const& s) const
        {
                return m_slices[get_material(s)].find(s);
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
        auto enumerate_max(int n)
        {
                for (auto i = 2; i <= n; ++i) {
                        enumerate(i);
                }
        }

        auto enumerate(int i)
        {
                assert(2 <= i);
                for (auto b = 1; b < i; ++b) {
                        enumerate(b, i - b);
                }
        }

        auto enumerate(int b, int w)
        {
                assert(1 <= b); assert(1 <= w);
                for (auto bp = 0; bp <= b; ++bp) {
                        for (auto wp = 0; wp <= w; ++wp) {
                                enumerate(bp, b - bp, wp, w - wp);
                        }
                }
        }

        auto enumerate(int bp, int bk, int wp, int wk)
        {
                auto const m = material{{bp, bk, wp, wk}};
                m_slices.push_back(m);
        }

        auto all_edges()
        {
                for (auto i = 0u; i < m_slices.size(); ++i) {
                        for (auto j = i + 1; j < m_slices.size(); ++j) {
                                if (depends_on{}(m_slices[i], m_slices[j])) {
                                        boost::add_edge(i, j, m_graph);
                                }
                                if (depends_on{}(m_slices[j], m_slices[i])) {
                                        boost::add_edge(j, i, m_graph);
                                }
                        }
                }
                m_index = boost::get(boost::vertex_index, m_graph);
        }
};

int main()
{
        database<> db;
        db.solve(3);
        db.print_slices();
        db.print_vertices();
        db.print_edges();
        db.sort();
}
