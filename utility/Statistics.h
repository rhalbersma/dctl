#include "../utility/IntegerTypes.h"

namespace dctl {

class Statistics
{
public:
        Statistics();

        // views
        NodeCount nodes() const;
        NodeCount sum_ply() const;
        int max_ply() const;

        // modifiers
        void reset();
        void update(int);

private:
        // representation
        NodeCount nodes_;
        NodeCount sum_ply_;
        int max_ply_;
};

}       // namespace dctl
