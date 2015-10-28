#ifndef DATE_HPP_KELLY
#define DATE_HPP_KELLY

namespace Kelly
{
    class Date
    {
        int _days;

    public:
        constexpr Date() : _days(0) {}
        explicit constexpr Date(int days) : _days(days) {}
        Date(int year, int month, int day);
        constexpr Date(const Date&) = default;
        ~Date() = default;

        Date& operator=(const Date&) = default;
    };
}

#endif
