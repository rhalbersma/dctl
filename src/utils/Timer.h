#pragma once
#include <ctime>
#include <vector>

namespace dctl {

class Timer
{
public:
        Timer();                    // records time of construction
        void split();
        void reset();

        clock_t elapsed() const;    // milliseconds since construction
        clock_t lap() const;        // milliseconds since last split()

private:
        // representation
        std::vector<clock_t> laps_;     // lap times
};

}       // namespace dctl
