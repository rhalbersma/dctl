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
        NodeCount nodes_;
        NodeCount sum_ply_;
        size_t max_ply_;
};