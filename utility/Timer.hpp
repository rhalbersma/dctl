#pragma once
#include <ctime>                        // clock_t
#include <vector>                       // std::vector
#include <boost/assert.hpp>             // BOOST_ASSERT

namespace dctl {

class Timer
{
public:
        Timer()
        {
                split();                // record time of construction
        }    

        void split()
        {
                laps_.push_back(clock());        
        }

        void reset()
        {
                laps_.clear();        
        }

        // milliseconds since construction
        clock_t elapsed() const
        {
                return laps_.back() - laps_.front();
        }    

        // milliseconds since last split()
        clock_t Timer::lap() const
        {
                BOOST_ASSERT(!laps_.empty());
                const auto i = laps_.size() - 1;
                return laps_[i] - laps_[i - 1];
        }
        
private:
        // representation
        std::vector<clock_t> laps_;     // lap times
};

}       // namespace dctl
