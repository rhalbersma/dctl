#pragma once
#include <ctime>
#include <vector>

class Timer
{
public:
        Timer(void);                    // records time of construction
        void split(void);
        void reset(void);

        clock_t elapsed(void) const;    // milliseconds since construction
        clock_t lap(void) const;        // milliseconds since last split()

private:
        // representation
        std::vector<clock_t> laps_;     // lap times
};
