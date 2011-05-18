#pragma once
#include <ctime>
#include <vector>

class Timer
{
public:
        Timer(void);                    // records time of construction
        void split(void);
        void reset(void);

        double elapsed(void) const;     // seconds since construction
        double lap(void) const;         // seconds since last split()

private:
        // implementation
        double seconds(clock_t) const;  // convert clock() ticks to seconds

        // representation
        std::vector<clock_t> laps_;     // lap times
};
