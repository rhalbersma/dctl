#include <boost/assert.hpp>
#include "Timer.h"

namespace dctl {

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
        BOOST_ASSERT(!laps_.empty());
        const auto i = laps_.size() - 1;
        return laps_[i] - laps_[i - 1];
}

}       // namespace dctl
