#include "../../include/Clock.hpp"
#include <ctime>

namespace Kelly
{
    void Sleep(const TimeSpan timeSpan)
    {
        timespec ts = {};
        ts.tv_sec = timeSpan.ToSeconds();
        ts.tv_nsec = timeSpan.ToNanoseconds() % 1000000000L;
        while (nanosleep(&ts, &ts) == -1) continue;
    }
}
