#include "../include/Tools.hpp"
#include "../include/View.hpp"
#include "../include/Stopwatch.hpp"
#include "../include/DateTime.hpp"
#include "../include/Clock.hpp"
#include "../include/Rotation.hpp"
#include "../include/CString.hpp"
#include <iostream>
#include <iomanip>
#include <map>
using namespace std;
using namespace Kelly;

void Test(CString cs)
{
    if (cs.HasContent())
        cout << "value: " << cs << endl;
    else
        cout << "no value" << endl;
}

void TestThings()
{
    CString n = "SUP";
    cout << sizeof(ptrdiff_t) << endl;
    cout << "TEST: " << n << endl;
    cout << "TEST: " << n + 1 << endl;
    cout << "char: " << n[1] << endl;
    cout << "length: " << n.Length() << endl;

    cout << "sizeof bool math: " << sizeof(true - false) << endl;

    Test(nullptr);

    cout << "yes?" << endl;

    map<CString, CString> options;

    string blam = "huzzah";

    options["database"] = "sqlite";
    options["z-index"] = "7";
    options["alt"] = "enable";
    options["wow"];
    options[blam] = "pi";
    options[nullptr] = "AAA";
    options[""] = "BBB";

    for (auto i : options)
        cout << "option -- {" << i.first << "} : [" << i.second << "]\n";

    cout << "I want the database: " << options["database"] << endl;
}

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

void TestView()
{
    char buffer[] = "GREETINGS";
    const View<char> text = { buffer, 5 };
    text.data[0] = 'g';

    for (auto i : text) cout << i;
    cout << endl;

    int aa = 0xdeadbeef;
    auto bb = Get<float>(&aa);
    cout << aa << " --> " << bb << endl;

    bb = 8.25f;
    aa = Get<int>(&bb);
    cout << bb << " --> " << aa << endl;

    cout << hex << aa << " --> " << EndianSwapped(aa) << endl;
    cout << "Just checking: " << aa << endl;
    cout << dec;
}

int main(int argc, char** argv)
{
    TestView();
    return 0;
}
