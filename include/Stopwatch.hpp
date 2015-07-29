#ifndef STOPWATCH_HPP_KELLY
#define STOPWATCH_HPP_KELLY

#include "TimeSpan.hpp"

namespace Kelly
{
    class Stopwatch
    {
        TimeSpan _elapsed;
        TimeSpan _start;
        bool _isRunning = false;

    public:
        constexpr Stopwatch() = default;
        constexpr Stopwatch(const Stopwatch&) = default;
        ~Stopwatch() = default;

        Stopwatch& operator=(const Stopwatch&) = default;

        void Start();
        void Stop();
        void Reset();
        void Restart();

        const TimeSpan Elapsed() const;
        inline bool IsRunning() const { return _isRunning; }
    };
}

#endif
