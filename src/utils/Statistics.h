#include "../utils/IntegerTypes.h"

namespace dtl {

class Statistics
{
public:
        Statistics();

        // views
        NodeCount nodes() const;
        NodeCount sum_ply() const;
        size_t max_ply() const;

        // modifiers
        void reset();
        void update(size_t);

private:
        // representation
        NodeCount nodes_;
        NodeCount sum_ply_;
        size_t max_ply_;
};

}       // namespace dtl
