#include "../Utilities/IntegerTypes.h"

class SearchStatistics
{
public:
        // views
        NodeCount nodes(void) const;
        NodeCount sum_ply(void) const;
        size_t max_ply(void) const;

        // modifiers
        void reset(void);
        void update(size_t);

private:
        // representation
        NodeCount d_nodes;
        NodeCount d_sum_ply;
        size_t d_max_ply;
};
