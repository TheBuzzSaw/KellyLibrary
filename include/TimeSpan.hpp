#ifndef TIMESPAN_HPP
#define TIMESPAN_HPP

#include <cstdint>
#include <iostream>
#include "Math.hpp"

namespace Kelly
{
    constexpr int64_t LowestValue = int64_t(1) << 63;
    constexpr int64_t NanosecondsPerTick = 100;
    constexpr int64_t TicksPerMicrosecond = 10;
    constexpr int64_t TicksPerMillisecond = TicksPerMicrosecond * 1000;
    constexpr int64_t TicksPerSecond = TicksPerMillisecond * 1000;
    constexpr int64_t TicksPerMinute = TicksPerSecond * 60;
    constexpr int64_t TicksPerHour = TicksPerMinute * 60;
    constexpr int64_t TicksPerDay = TicksPerHour * 24;
    constexpr int64_t TicksPerWeek = TicksPerDay * 7;

    class TimeSpan
    {
        int64_t _ticks;

    public:
        constexpr TimeSpan() : _ticks(0) {}
        explicit constexpr TimeSpan(int64_t ticks) : _ticks(ticks) {}
        TimeSpan(int hour, int minute, int second);
        constexpr TimeSpan(const TimeSpan&) = default;
        ~TimeSpan() = default;

        constexpr int64_t Ticks() const { return _ticks; }

        TimeSpan& operator=(const TimeSpan&) = default;
        TimeSpan& operator+=(TimeSpan other);
        TimeSpan& operator-=(TimeSpan other);

        constexpr int64_t ToNanoseconds() const
        {
            return _ticks * NanosecondsPerTick;
        }

        constexpr int64_t ToMicroseconds() const
        {
            return _ticks / TicksPerMicrosecond;
        }

        constexpr int64_t ToMilliseconds() const
        {
            return _ticks / TicksPerMillisecond;
        }

        constexpr int64_t ToSeconds() const
        {
            return _ticks / TicksPerSecond;
        }

        constexpr int64_t ToMinutes() const
        {
            return _ticks / TicksPerMinute;
        }

        constexpr int64_t ToHours() const
        {
            return _ticks / TicksPerHour;
        }

        constexpr int64_t ToDays() const
        {
            return _ticks / TicksPerDay;
        }

        constexpr int64_t ToWeeks() const
        {
            return _ticks / TicksPerWeek;
        }

        static constexpr TimeSpan FromNanoseconds(int64_t nanoseconds)
        {
            return TimeSpan(nanoseconds / NanosecondsPerTick);
        }

        static constexpr TimeSpan FromMicroseconds(int64_t microseconds)
        {
            return TimeSpan(microseconds * TicksPerMicrosecond);
        }

        static constexpr TimeSpan FromMilliseconds(int64_t milliseconds)
        {
            return TimeSpan(milliseconds * TicksPerMillisecond);
        }

        static constexpr TimeSpan FromSeconds(int64_t seconds)
        {
            return TimeSpan(seconds * TicksPerSecond);
        }

        static constexpr TimeSpan FromMinutes(int64_t minutes)
        {
            return TimeSpan(minutes * TicksPerMinute);
        }

        static constexpr TimeSpan FromHours(int64_t hours)
        {
            return TimeSpan(hours * TicksPerHour);
        }

        static constexpr TimeSpan FromDays(int64_t days)
        {
            return TimeSpan(days * TicksPerDay);
        }

        static constexpr TimeSpan FromWeeks(int64_t weeks)
        {
            return TimeSpan(weeks * TicksPerWeek);
        }

        static constexpr TimeSpan MinValue()
        {
            return TimeSpan(LowestValue);
        }

        static constexpr TimeSpan Zero()
        {
            return TimeSpan(0);
        }

        static constexpr TimeSpan MaxValue()
        {
            return TimeSpan(~LowestValue);
        }
    };

    constexpr bool operator==(TimeSpan a, TimeSpan b)
    {
        return a.Ticks() == b.Ticks();
    }

    constexpr bool operator!=(TimeSpan a, TimeSpan b)
    {
        return a.Ticks() != b.Ticks();
    }

    constexpr bool operator<(TimeSpan a, TimeSpan b)
    {
        return a.Ticks() < b.Ticks();
    }

    constexpr bool operator<=(TimeSpan a, TimeSpan b)
    {
        return a.Ticks() <= b.Ticks();
    }

    constexpr bool operator>(TimeSpan a, TimeSpan b)
    {
        return a.Ticks() > b.Ticks();
    }

    constexpr bool operator>=(TimeSpan a, TimeSpan b)
    {
        return a.Ticks() >= b.Ticks();
    }

    constexpr TimeSpan operator+(TimeSpan a, TimeSpan b)
    {
        return TimeSpan(a.Ticks() + b.Ticks());
    }

    constexpr TimeSpan operator-(TimeSpan a, TimeSpan b)
    {
        return TimeSpan(a.Ticks() - b.Ticks());
    }

    constexpr TimeSpan operator-(TimeSpan ts)
    {
        return TimeSpan(-ts.Ticks());
    }

    template<typename CharT, typename TraitsT>
    std::basic_ostream<CharT, TraitsT>& operator<<(
        std::basic_ostream<CharT, TraitsT>& stream,
        TimeSpan ts)
    {
        int64_t n = ts.ToWeeks();
        stream << n << 'w';

        n = ts.ToDays() - ts.ToWeeks() * 7;
        stream << n << 'd';

        n = ts.ToHours() - ts.ToDays() * 24;
        stream << n << 'h';

        n = ts.ToMinutes() - ts.ToHours() * 60;
        stream << n << 'm';

        n = ts.ToSeconds() - ts.ToMinutes() * 60;
        stream << n << 's';

        n = ts.ToMilliseconds() - ts.ToSeconds() * 1000;
        stream << n << "ms";

        n = ts.ToMicroseconds() - ts.ToMilliseconds() * 1000;
        stream << n << "us";

        return stream;
    }
}

#endif
