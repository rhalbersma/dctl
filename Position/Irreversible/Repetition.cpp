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
        return d_repetition.rbegin();
}

Repetition::const_reverse_iterator Repetition::rend(void) const
{
        return d_repetition.rend();
}

HashIndex& Repetition::hash_index(void)
{
        return d_repetition.back();
}

HashIndex Repetition::hash_index(void) const
{
        return d_repetition.back();
}

void Repetition::copy_back(void)
{
        d_repetition.copy_back();
}

void Repetition::push_back(HashIndex i)
{
        d_repetition.push_back(i);
}

void Repetition::pop_back(void)
{
        d_repetition.pop_back();
}
