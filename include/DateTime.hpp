#ifndef DATETIME_HPP
#define DATETIME_HPP

#include "TimeSpan.hpp"

namespace Kelly
{
    constexpr int64_t MinTickCount = 0LL;
    constexpr int64_t MaxTickCount = 3155378975999999999LL;

    constexpr int64_t SafeTicks(int64_t ticks)
    {
        return Bound(ticks, MinTickCount, MaxTickCount);
    }

    class DateTime
    {
        int64_t _ticks;

    public:
        constexpr DateTime() : _ticks(0) {}
        explicit constexpr DateTime(int64_t ticks)
            : _ticks(SafeTicks(ticks)) {}
        DateTime(
            int year,
            int month,
            int day,
            int hour = 0,
            int minute = 0,
            int second = 0,
            int millisecond = 0,
            int microsecond = 0,
            int ticks = 0);
        constexpr DateTime(const DateTime&) = default;
        ~DateTime() = default;

        constexpr int64_t Ticks() const { return _ticks; }
        constexpr operator bool() const { return _ticks > 0; }

        DateTime& operator=(const DateTime&) = default;
        DateTime& operator+=(TimeSpan timeSpan);
        DateTime& operator-=(TimeSpan timeSpan);

        constexpr TimeSpan TimeOfDay() const
        {
            return TimeSpan(_ticks % TicksPerDay);
        }

        constexpr DateTime Date() const
        {
            return DateTime((_ticks / TicksPerDay) * TicksPerDay);
        }

        constexpr int DayOfWeek() const
        {
            return (_ticks / TicksPerDay) % 7;
        }

        int Year() const;
        int Month() const;
        int Day() const;

        constexpr int Hour() const
        {
            return (_ticks / TicksPerHour) % 24;
        }

        constexpr int Minute() const
        {
            return (_ticks / TicksPerMinute) % 60;
        }

        constexpr int Second() const
        {
            return (_ticks / TicksPerSecond) % 60;
        }

        constexpr int Millisecond() const
        {
            return (_ticks / TicksPerMillisecond) % 1000;
        }

        constexpr int Microsecond() const
        {
            return (_ticks / TicksPerMicrosecond) % 1000;
        }

        static int DaysInMonth(int month, int year = 1);

        static const char* DayToString(int dayOfWeek);
        static DateTime LocalTime();
        static DateTime UtcTime();

        static constexpr bool IsLeapYear(int year)
        {
            return !(year % 4) && ((year % 100) || !(year % 400));
        }

        static constexpr int DaysInYear(int year)
        {
            return 365 + IsLeapYear(year);
        }

        static constexpr DateTime MinValue()
        {
            return DateTime(MinTickCount);
        }

        static constexpr DateTime MaxValue()
        {
            return DateTime(MaxTickCount);
        }
    };

    constexpr bool operator==(DateTime a, DateTime b)
    {
        return a.Ticks() == b.Ticks();
    }

    constexpr bool operator!=(DateTime a, DateTime b)
    {
        return a.Ticks() != b.Ticks();
    }

    constexpr bool operator<(DateTime a, DateTime b)
    {
        return a.Ticks() < b.Ticks();
    }

    constexpr bool operator<=(DateTime a, DateTime b)
    {
        return a.Ticks() <= b.Ticks();
    }

    constexpr bool operator>(DateTime a, DateTime b)
    {
        return a.Ticks() > b.Ticks();
    }

    constexpr bool operator>=(DateTime a, DateTime b)
    {
        return a.Ticks() >= b.Ticks();
    }

    constexpr DateTime operator+(DateTime a, TimeSpan b)
    {
        return DateTime(a.Ticks() + b.Ticks());
    }

    constexpr DateTime operator-(DateTime a, TimeSpan b)
    {
        return DateTime(a.Ticks() - b.Ticks());
    }

    constexpr TimeSpan operator-(DateTime a, DateTime b)
    {
        return TimeSpan(a.Ticks() - b.Ticks());
    }

    template<typename CharT, typename TraitsT>
    std::basic_ostream<CharT, TraitsT>& operator<<(
        std::basic_ostream<CharT, TraitsT>& stream,
        DateTime dateTime)
    {
        stream
            << dateTime.Year()
            << '-'
            << dateTime.Month()
            << '-'
            << dateTime.Day()
            << ' ';

        stream << dateTime.Hour() << ':';

        int minute = dateTime.Minute();
        if (minute < 10) stream << '0';
        stream << minute << ':';

        int second = dateTime.Second();
        if (second < 10) stream << '0';
        stream << second;

        return stream;
    }
}

#endif
