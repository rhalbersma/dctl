#include "../Utilities/IntegerTypes.h"

class SearchStatistics
{
public:
        // views
        uint64_t nodes(void) const;
        uint64_t sum_ply(void) const;
        size_t max_ply(void) const;

        // modifiers
        void reset(void);
        void update(void);
        void update(size_t);

private:
        // representation
        uint64_t d_nodes;
        uint64_t d_sum_ply;
        size_t d_max_ply;
};
