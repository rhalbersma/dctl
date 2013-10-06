#pragma once
#include <chrono>                       // clock_t
#include <vector>                       // vector

namespace dctl {

class Timer
{
        using clock = std::chrono::high_resolution_clock;

public:
        Timer()
        {
                split();
        }

        void split()
        {
                splits_.push_back(clock::now());
        }

        void reset()
        {
                splits_.clear();
        }

        // milliseconds since first split()
        std::chrono::milliseconds elapsed() const
        {
                return std::chrono::duration_cast<std::chrono::milliseconds>(splits_.back() - splits_.front());
        }

        // milliseconds since last split()
        std::chrono::milliseconds lap() const
        {
                auto const i = splits_.size() - 1;
                return std::chrono::duration_cast<std::chrono::milliseconds>(splits_.back() - splits_[i - 1]);
        }

private:
        // representation

        std::vector<std::chrono::high_resolution_clock::time_point> splits_;
};

}       // namespace dctl
