#pragma once
#include <vector>                       // std::vector

namespace dctl {
namespace search {

typedef std::vector<int> Sequence;
typedef std::vector<int> Order;

class Parameters
{
public:
        // constructors
        Parameters()
        {
                pv_.reserve(32);
        }

        // views
        const Sequence& pv() const
        {
                return pv_;
        }

        int best_move() const
        {
                return *pv_.begin();
        }

        // modifiers
        void set_pv(int first_move, const Sequence& continuation)
        {
                pv_.clear();
                pv_.push_back(first_move);
                pv_.insert(pv_.end(), continuation.begin(), continuation.end());
        }

private:
        // representation
        Sequence pv_;
};

}       // namespace search
}       // namespace dctl
