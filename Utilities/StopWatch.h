#pragma once
#include <ctime>
#include <vector>

class StopWatch
{
public:
        void start(void);
        void split(void);
        void reset(void);

        double total(void) const;       // seconds since start()
        double delta(void) const;       // seconds since last split()

private:
        // implementation
        double seconds(clock_t) const;  // convert clock() ticks to seconds

        // representation
        std::vector<clock_t> split_;   // split() history
};
