#include "StopWatch.h"
#include <cassert>

void StopWatch::start(void)
{
        d_split.push_back(clock());
}

void StopWatch::split(void)
{
        d_split.push_back(clock());        
}

void StopWatch::reset(void)
{
        d_split.clear();        
}

double StopWatch::total(void) const
{
        return seconds(d_split.end() - d_split.begin());
}

double StopWatch::delta(void) const
{
        assert(!d_split.empty());
        const size_t i = d_split.size() - 1;
        return seconds(d_split[i] - d_split[i - 1]);
}

double StopWatch::seconds(clock_t clocks) const
{
        return static_cast<double>(clocks) / CLOCKS_PER_SEC;
}
