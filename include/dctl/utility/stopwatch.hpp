#pragma once
#include <dctl/utility/logical.hpp>     // implies
#include <cassert>                      // assert
#include <chrono>                       // duration_cast, high_resolution_clock, milliseconds
#include <vector>                       // vector

namespace dctl {

class Stopwatch
{
        using units = std::chrono::milliseconds;
        using clock = std::chrono::high_resolution_clock;

        std::vector<clock::time_point> splits;
        bool is_running = false;

        auto invariant() const
        {
                return
                        implies(splits.empty()    , !is_running) &&
                        implies(splits.size() == 1,  is_running)
                ;
        }

public:
        Stopwatch() noexcept
        {
                assert(invariant());
        }

        void start_stop()
        {
                if (!is_running)
                        start();
                else
                        stop();
        }

        void split_reset()
        {
                if (is_running)
                        split();
                else
                        reset();
        }

        units elapsed_time() const noexcept
        {
                using namespace std::chrono;
                return
                        is_running ?
                        duration_cast<units>(clock::now() - splits.front()) :
                        split_time()
                ;
        }

        units split_time() const noexcept
        {
                using namespace std::chrono;
                auto const N = splits.size();
                switch (N) {
                case 0  : return units::zero();
                case 1  : return duration_cast<units>(clock::now()  - splits.front());
                default : return duration_cast<units>(splits.back() - splits.front());
                }
        }

        units lap_time() const noexcept
        {
                using namespace std::chrono;
                auto const N = splits.size();
                switch (N) {
                case 0  : return units::zero();
                case 1  : return duration_cast<units>(clock::now()  - splits[N - 1]);
                default : return duration_cast<units>(splits[N - 1] - splits[N - 2]);
                }
        }

private:
        void start()
        {
                assert(!is_running);
                is_running = true;
                split();
                assert(invariant());
        }

        void stop()
        {
                assert(is_running);
                split();
                is_running = false;
                assert(invariant());
        }

        void split()
        {
                assert(is_running);
                splits.push_back(clock::now());
                assert(invariant());
        }

        void reset()
        {
                assert(!is_running);
                splits.clear();
                assert(invariant());
        }
};

}       // namespace dctl
