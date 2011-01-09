#include "Repetition.h"
#include <algorithm>
#include <iterator>

// C++ STL does not provide (reverse) stride iterators, so we have a hand-crafted loop here
bool Repetition::non_unique_back(size_t max_cycle) const
{
        if (max_cycle < MIN_CYCLE)
                return false;

        const_reverse_iterator ri_max = std::min(rbegin() + max_cycle, rend());
        for (const_reverse_iterator ri = rbegin() + MIN_CYCLE; ri <= ri_max; ri += STRIDE) {
                if (*ri == hash_index())
                        return true;
        }
        return false;
}

Repetition::const_reverse_iterator Repetition::rbegin(void) const
{
        return repetition_.rbegin();
}

Repetition::const_reverse_iterator Repetition::rend(void) const
{
        return repetition_.rend();
}

HashIndex& Repetition::hash_index(void)
{
        return repetition_.back();
}

HashIndex Repetition::hash_index(void) const
{
        return repetition_.back();
}

void Repetition::copy_back(void)
{
        repetition_.copy_back();
}

void Repetition::push_back(HashIndex i)
{
        repetition_.push_back(i);
}

void Repetition::pop_back(void)
{
        repetition_.pop_back();
}
