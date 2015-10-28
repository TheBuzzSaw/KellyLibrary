#include "../include/DateTime.hpp"
#include <ctime>

namespace Kelly
{
    static const int DaysInMonths[] =
        { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    static constexpr int DaysPerYear = 365;
    static constexpr int TicksPerYear = TicksPerDay * DaysPerYear;
    static constexpr int DaysPerFourCenturies = DaysPerYear * 400 + 97;
    static constexpr int DaysPerCentury = DaysPerYear * 100 + 24;
    static constexpr int DaysPerFourYears = DaysPerYear * 4 + 1;

    std::pair<int, int> ExtractYears(int days)
    {
        int year = 1;

        if (days >= DaysPerFourCenturies)
        {
            int chunks = days / DaysPerFourCenturies;
            year += chunks * 400;
            days -= chunks * DaysPerFourCenturies;
        }

        if (days >= DaysPerCentury)
        {
            int chunks = days / DaysPerCentury;
            if (chunks == 4) chunks = 3;
            year += chunks * 100;
            days -= chunks * DaysPerCentury;
        }

        if (days >= DaysPerFourYears)
        {
            int chunks = days / DaysPerFourYears;
            year += chunks * 4;
            days -= chunks * DaysPerFourYears;
        }

        if (days >= DaysPerYear)
        {
            int chunks = days / DaysPerYear;
            if (chunks == 4) chunks = 3;
            year += chunks;
            days -= chunks * DaysPerYear;
        }

        return {year, days};
    }

    std::pair<int, int> ExtractMonth(int days, int year)
    {
        int month = 1;

        for (int daysInMonth = DateTime::DaysInMonth(month, year);
            days >= daysInMonth;
            daysInMonth = DateTime::DaysInMonth(month, year))
        {
            ++month;
            days -= daysInMonth;
        }

        return {month, days};
    }

    DateTime::DateTime(
        int year,
        int month,
        int day,
        int hour,
        int minute,
        int second,
        int millisecond,
        int microsecond,
        int ticks)
    {
        if (InRange(year, 1, 9999) &&
            InRange(day, 1, DaysInMonth(month, year)) &&
            InRange(hour, 0, 23) &&
            InRange(minute, 0, 59) &&
            InRange(second, 0, 59) &&
            InRange(millisecond, 0, 999) &&
            InRange(microsecond, 0, 999) &&
            InRange(ticks, 0, 9))
        {
            int64_t days = day - 1;

            for (int i = 1; i < month; ++i)
                days += DaysInMonth(i, year);

            --year;
            days +=
                (year * DaysPerYear) +
                (year / 4) -
                (year / 100) +
                (year / 400);

            _ticks =
                days * TicksPerDay +
                hour * TicksPerHour +
                minute * TicksPerMinute +
                second * TicksPerSecond +
                millisecond * TicksPerMillisecond +
                microsecond * TicksPerMicrosecond +
                ticks;
        }
        else
        {
            _ticks = 0;
        }
    }

    int DateTime::Year() const
    {
        int days = _ticks / TicksPerDay;
        return ExtractYears(days).first;
    }

    int DateTime::Month() const
    {
        int days = _ticks / TicksPerDay;
        auto y = ExtractYears(days);
        return ExtractMonth(y.second, y.first).first;
    }

    int DateTime::Day() const
    {
        int64_t days = _ticks / TicksPerDay;
        auto y = ExtractYears(days);
        auto m = ExtractMonth(y.second, y.first);
        return m.second + 1;
    }

    DateTime& DateTime::operator+=(TimeSpan timeSpan)
    {
        _ticks = SafeTicks(_ticks += timeSpan.Ticks());
        return *this;
    }

    DateTime& DateTime::operator-=(TimeSpan timeSpan)
    {
        _ticks = SafeTicks(_ticks -= timeSpan.Ticks());
        return *this;
    }

    int DateTime::DaysInMonth(int month, int year)
    {
        if (InRange(month, 1, 12))
        {
            if (month == 2 && IsLeapYear(year))
                return 29;

            return DaysInMonths[month];
        }

        return 0;
    }

    const char* DateTime::DayToString(int dayOfWeek)
    {
        const char* DayNames[7] = {
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday",
            "Sunday"};

        return InRange(dayOfWeek, 0, 6) ? DayNames[dayOfWeek] : nullptr;
    }

    static DateTime GetDateTime(const tm& timeInfo)
    {
        return DateTime(
            timeInfo.tm_year + 1900,
            timeInfo.tm_mon + 1,
            timeInfo.tm_mday,
            timeInfo.tm_hour,
            timeInfo.tm_min,
            Min(timeInfo.tm_sec, 59)); // Can exceed 59! :(

        // http://www.cplusplus.com/reference/clibrary/ctime/tm/
    }

    DateTime DateTime::LocalTime()
    {
        time_t rawTime;
        time(&rawTime);
        return GetDateTime(*localtime(&rawTime));
    }

    DateTime DateTime::UtcTime()
    {
        time_t rawTime;
        time(&rawTime);
        return GetDateTime(*gmtime(&rawTime));
    }

}
