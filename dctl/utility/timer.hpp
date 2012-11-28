#pragma once
#include <chrono>                       // clock_t
#include <vector>                       // vector
#include <boost/assert.hpp>             // BOOST_ASSERT

namespace dctl {

class Timer
{
        typedef std::chrono::high_resolution_clock clock;

public:
        Timer()
        {
                split();                // record time of construction
        }

        void split()
        {
                laps_.push_back(clock::now());
        }

        void reset()
        {
                laps_.clear();
        }

        // milliseconds since construction
        std::chrono::milliseconds elapsed() const
        {
                return std::chrono::duration_cast<std::chrono::milliseconds>(laps_.back() - laps_.front());
        }

        // milliseconds since last split()
        std::chrono::milliseconds lap() const
        {
                BOOST_ASSERT(!laps_.empty());
                auto const i = laps_.size() - 1;
                return std::chrono::duration_cast<std::chrono::milliseconds>(laps_[i] - laps_[i - 1]);
        }

private:
        // representation
        std::vector<std::chrono::high_resolution_clock::time_point> laps_;     // lap times
};

}       // namespace dctl
