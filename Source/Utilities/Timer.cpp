#include <cassert>
#include "Timer.h"

Timer::Timer(void)
{
        split();
}

void Timer::split(void)
{
        laps_.push_back(clock());        
}

void Timer::reset(void)
{
        laps_.clear();        
}

clock_t Timer::elapsed(void) const
{
        return laps_.back() - laps_.front();
}

clock_t Timer::lap(void) const
{
        assert(!laps_.empty());
        const size_t i = laps_.size() - 1;
        return laps_[i] - laps_[i - 1];
}
