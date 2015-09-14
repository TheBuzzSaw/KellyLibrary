#include "../../include/Clock.hpp"
#include <ctime>

namespace Kelly
{
    void Sleep(TimeSpan timeSpan)
    {
        int64_t nanoseconds = timeSpan.ToNanoseconds();
        int64_t seconds = nanoseconds / 1e9;

        timespec ts = {};
        ts.tv_sec = seconds;
        ts.tv_nsec = nanoseconds - seconds * 1e9;
        while (nanosleep(&ts, &ts) == -1) continue;
    }
}
