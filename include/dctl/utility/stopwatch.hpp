#pragma once
#include <cassert>                      // assert
#include <chrono>                       // clock_t
#include <vector>                       // vector

namespace dctl {

class Stopwatch
{
public:
        Stopwatch() = default;

        void start()
        {
                assert(!running_);
                record();
                running_ = true;
        }

        void stop()
        {
                assert(running_);
                record();
                running_ = false;
        }

        void lap()
        {
                assert(running_);
                record();
        }

        void reset()
        {
                assert(!running_);
                splits_.clear();
        }

        auto time() const
        {
                using namespace std::chrono;

                return duration_cast<milliseconds>(splits_.back() - splits_.front());
        }

        auto split() const
        {
                using namespace std::chrono;

                assert(splits_.size() > 1);
                auto const i = splits_.size() - 1;
                return duration_cast<milliseconds>(splits_[i] - splits_[i - 1]);
        }

private:
        // implementation

        void record()
        {
                splits_.push_back(clock::now());
        }

        // representation

        using clock = std::chrono::high_resolution_clock;
        std::vector<clock::time_point> splits_{};
        bool running_{};
};

}       // namespace dctl
