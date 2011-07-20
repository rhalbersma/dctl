#include <cassert>
#include "Timer.h"

Timer::Timer()
{
        split();
}

void Timer::split()
{
        laps_.push_back(clock());        
}

void Timer::reset()
{
        laps_.clear();        
}

clock_t Timer::elapsed() const
{
        return laps_.back() - laps_.front();
}

clock_t Timer::lap() const
{
        assert(!laps_.empty());
        const size_t i = laps_.size() - 1;
        return laps_[i] - laps_[i - 1];
}
