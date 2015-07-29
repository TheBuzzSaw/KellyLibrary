#include "../include/Stopwatch.hpp"
#include "../include/DateTime.hpp"
#include "../include/Clock.hpp"
#include "../include/Rotation.hpp"
#include <iostream>
using namespace std;
using namespace Kelly;

void TestDateTimeStuff()
{
    cout << "size: " << sizeof(DateTime) << endl;
    cout << "Max: " << TimeSpan::MaxValue().Ticks() << endl;
    cout << "Min: " << TimeSpan::MinValue().Ticks() << endl;

    DateTime a = DateTime::LocalTime();
    DateTime b = a.Date();

    TimeSpan gap = a - b;

    cout << gap << endl;

    a += TimeSpan::FromMinutes(5);

    int day = a.DayOfWeek();
    cout << DateTime::DayToString(day) << endl;
    cout << "5 minutes from now: " << a << endl;
    cout << "today with date only: " << a.Date() << endl;
    cout << "standard UTC time: " << DateTime::UtcTime() << endl;
    cout << "native UTC time: " << GetNativeTime() << endl;

    a = DateTime(2012, 12, 31);
    cout << "Dec 31, yes? " << a << endl;
    cout << a.Ticks() << endl;

    ResetTimer();
    Sleep(TimeSpan::FromSeconds(2));
    cout << ReadTimer() << endl;
}

int main(int argc, char** argv)
{
    Stopwatch sw;
    sw.Start();
    Rotation64 r = Rotation64::FromDegrees(22.5);
    cout << r.ToDegrees() << endl;
    cout << Pi<double>() << endl;

    Sleep(TimeSpan::FromSeconds(3));

    cout << sw.Elapsed() << endl;

    sw.Stop();

    cout << sw.Elapsed() << endl;

    //TestDateTimeStuff();
    return 0;
}
