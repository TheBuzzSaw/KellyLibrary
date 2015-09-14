#ifndef CLOCK_HPP_KELLY
#define CLOCK_HPP_KELLY

#include "DateTime.hpp"

namespace Kelly
{
    void Sleep(TimeSpan timeSpan);
    DateTime GetNativeTime();
    void ResetTimer();
    TimeSpan ReadTimer();
}

#endif
