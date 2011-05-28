#include "../Utilities/IntegerTypes.h"

namespace Tree {

class Statistics
{
public:
        // views
        NodeCount nodes(void) const;
        NodeCount sum_ply(void) const;
        size_t max_ply(void) const;
        NodeCount pv(void) const;

        // modifiers
        void reset(void);
        void update(size_t);
        void update_pv(void);

private:
        // representation
        NodeCount nodes_;
        NodeCount sum_ply_;
        size_t max_ply_;
        NodeCount pv_;
};

}       // namespace Tree
