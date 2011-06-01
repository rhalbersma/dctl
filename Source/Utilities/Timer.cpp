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

double Timer::elapsed(void) const
{
        return seconds(laps_.back() - laps_.front());
}

double Timer::lap(void) const
{
        assert(!laps_.empty());
        const size_t i = laps_.size() - 1;
        return seconds(laps_[i] - laps_[i - 1]);
}

double Timer::seconds(clock_t clocks) const
{
        return static_cast<double>(clocks) / CLOCKS_PER_SEC;
}
