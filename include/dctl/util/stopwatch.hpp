#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cassert>      // assert
#include <chrono>       // duration_cast, steady_clock, milliseconds
#include <vector>       // vector

namespace dctl {
namespace util {

class Stopwatch
{
        using units = std::chrono::milliseconds;
        using clock = std::chrono::steady_clock;

        std::vector<clock::time_point> splits;
        bool is_running = false;

        bool invariant() const noexcept
        {
                return
                        (not splits.empty()     || not is_running) &&
                        (    splits.size() != 1 ||     is_running)
                ;
        }

public:
        Stopwatch() noexcept
        {
                assert(invariant());
        }

        void start_stop()
        {
                if (not is_running) {
                        start();
                } else {
                        stop();
                }
                assert(invariant());
        }

        void split_reset()
        {
                if (is_running) {
                        split();
                } else {
                        reset();
                }
                assert(invariant());
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
                case 0: return units::zero();
                case 1: return duration_cast<units>(clock::now()  - splits.front());
                default: return duration_cast<units>(splits.back() - splits.front());
                }
        }

        units lap_time() const noexcept
        {
                using namespace std::chrono;
                auto const N = splits.size();
                switch (N) {
                case 0: return units::zero();
                case 1: return duration_cast<units>(clock::now()  - splits[N - 1]);
                default: return duration_cast<units>(splits[N - 1] - splits[N - 2]);
                }
        }

private:
        void start()
        {
                assert(not is_running);
                is_running = true;
                split();
        }

        void stop()
        {
                assert(is_running);
                split();
                is_running = false;
        }

        void split()
        {
                assert(is_running);
                splits.push_back(clock::now());
        }

        void reset()
        {
                assert(not is_running);
                splits.clear();
        }
};

}       // namespace util
}       // namespace dctl
