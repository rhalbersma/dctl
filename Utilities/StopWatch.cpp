#include "StopWatch.h"
#include <cassert>

void StopWatch::start(void)
{
        split_.push_back(clock());
}

void StopWatch::split(void)
{
        split_.push_back(clock());        
}

void StopWatch::reset(void)
{
        split_.clear();        
}

double StopWatch::total(void) const
{
        return seconds(split_.end() - split_.begin());
}

double StopWatch::delta(void) const
{
        assert(!split_.empty());
        const size_t i = split_.size() - 1;
        return seconds(split_[i] - split_[i - 1]);
}

double StopWatch::seconds(clock_t clocks) const
{
        return static_cast<double>(clocks) / CLOCKS_PER_SEC;
}
