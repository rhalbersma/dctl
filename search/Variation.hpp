#pragma once
#include <vector>                       // vector
#include <boost/assert.hpp>

namespace dctl {
namespace search {

typedef std::vector<int> Sequence;
typedef std::vector<int> Order;

class Variation
{
public:
        // views
        const Sequence& sequence() const
        {
                return sequence_;
        }

        int first_move() const
        {
                return *sequence_.begin();
        }

        // modifiers
        void clear()
        {
                sequence_.clear();
        }

        void set(int first_move, const Sequence& continuation)
        {
                clear();
                sequence_.push_back(first_move);
                sequence_.insert(sequence_.end(), continuation.begin(), continuation.end());
        }

private:
        // representation
        Sequence sequence_;
};

}       // namespace search
}       // namespace dctl
