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
        // queries
        const Sequence& sequence() const
        {
                return sequence_;
        }

        int first_move() const
        {
                return *std::begin(sequence_);
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
                sequence_.insert(std::end(sequence_), std::begin(continuation), std::end(continuation));
        }

private:
        // representation
        Sequence sequence_;
};

}       // namespace search
}       // namespace dctl
