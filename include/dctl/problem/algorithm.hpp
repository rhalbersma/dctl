
template<class Node, class Problem>
auto recursive_dls(Node const& n, Problem const& p, int limit)
{
        n.expand(problem);
}

template<class Problem>
auto depth_limited_search(Problem const& p, int limit)
{
        return recursive_dls(Node(p.initial), problem, limit);
}

template<class Problem>
auto iterative_deepening_search(Problem const& p)
{
        while (true) {
                result = depth_limited_search(problem, depth);

        }
}
