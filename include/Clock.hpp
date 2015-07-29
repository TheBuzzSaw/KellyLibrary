#ifndef CLOCK_HPP_KELLY
#define CLOCK_HPP_KELLY

#include "DateTime.hpp"

namespace Kelly
{
    void Sleep(const TimeSpan timeSpan);
    const DateTime GetNativeTime();
    void ResetTimer();
    const TimeSpan ReadTimer();
}

#endif
