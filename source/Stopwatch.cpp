#include "../include/Stopwatch.hpp"
#include "../include/Clock.hpp"

namespace Kelly
{
    void Stopwatch::Start()
    {
        if (_isRunning) return;

        _isRunning = true;
        _start = ReadTimer();
    }

    void Stopwatch::Stop()
    {
        if (!_isRunning) return;

        _elapsed += ReadTimer() - _start;
        _isRunning = false;
    }

    void Stopwatch::Reset()
    {
        _elapsed = TimeSpan::Zero();
        _isRunning = false;
    }

    void Stopwatch::Restart()
    {
        _elapsed = TimeSpan::Zero();
        _start = ReadTimer();
        _isRunning = true;
    }

    const TimeSpan Stopwatch::Elapsed() const
    {
        auto result = _elapsed;
        if (_isRunning) result += ReadTimer() - _start;
        return result;
    }
}
