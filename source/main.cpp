#include "../include/Network/Endpoint32Query.hpp"
#include "../include/Network/TcpListener.hpp"
#include "../include/Network/TcpConnection.hpp"
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
    const auto Message = "GREETINGS";
    View<const char> messageView = { Message, 9 };

    char buffer[6];
    const View<char> text = { buffer, sizeof(buffer) };

    Copy(text, messageView);
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

void TestSocket()
{
    Endpoint32Query query("yahoo.com", "80");

    for (auto i : query)
        cout << i << endl;

    auto endpoint = FindEndpoint32("google.com", "80");
    if (endpoint == NullEndpoint32)
    {
        cerr << "failed to locate Google\n";
        return;
    }

    TcpConnection connection(endpoint);
    if (!connection.IsOpen())
    {
        cerr << "failed to open socket\n";
        return;
    }

    char buffer[] =
        "GET / HTTP/1.1\r\n"
        "Host: www.google.com\r\n"
        "Connection: close\r\n"
        "\r\n";

    connection.Send(buffer, sizeof(buffer));

    ptrdiff_t n;
    while ((n = connection.Receive(buffer, sizeof(buffer))) > 0)
    {
        cout.write(buffer, n);
    }

    cout << endl;
}

void TestServer()
{
    TcpListener listener(8080);

    if (!listener.IsOpen())
    {
        cerr << "failed to listen on port " << listener.Endpoint().port << '\n';
        return;
    }

    cout << "listening on port " << listener.Endpoint().port << "..." << endl;
    auto connection = listener.Accept();
    if (connection.IsOpen())
    {
        cout << "connection established from " << connection.Endpoint() << endl;

        char buffer[1024];

        ptrdiff_t n;
        while ((n = connection.Receive(buffer, sizeof(buffer))) > 0)
        {
            cout.write(buffer, n);
            connection.SetBlocking(false);
        }

        auto response =
            "HTTP/1.1 200 OK\r\n"
            "Connection: close\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "\r\n"
            "<html><head><title>Kelly Server</title></head>"
            "<body><p>Hello, World! Text is da bomb.</p></body>"
            "</html>"
            ;

        strcpy(buffer, response);
        n = strlen(buffer);

        connection.Send(buffer, n);
    }
    else
    {
        cerr << "failed to accept\n";
        return;
    }
}

double Linear(double first, double last, double t)
{
    auto difference = last - first;
    return difference * t + first;
}

double Overshoot(double first, double last, double t)
{
    auto difference = last - first;
    auto magnitude = difference * -0.25;
    return difference * t + first + sin(t * Tau<double>()) * magnitude;
}

void TestInterpolation(double first, double last, int stepCount)
{
    cout << first << " to " << last << '\n';
    for (int i = 0; i <= stepCount; ++i)
    {
        auto t = double(i) / stepCount;

        cout << t
            << '\t' << Linear(first, last, t)
            << '\t' << Overshoot(first, last, t)
            << '\n';
    }

    cout << endl;
}

void TestRotation()
{
    auto a = Degrees<double>(45 + 360 + 360);
    auto b = Degrees<double>(-10 - 360 - 360);
    Rotation64 test(ToRadians(113.0));

    cout << test.ToDegrees() << endl;

    cout << a.ToDegrees() << endl;
    cout << b.ToDegrees() << endl;
    cout << (-a).ToDegrees() << endl;
    cout << (-b).ToDegrees() << endl;
    cout << (a + b).ToDegrees() << endl;
    cout << (a - b).ToDegrees() << endl;
    cout << (a + -b).ToDegrees() << endl;

    for (int i = 0; i < 8; ++i)
    {
        auto n = Pi<double>() * i / 4;
        cout << ToDegrees(n) << " --> " << sin(n) << endl;
    }
}

int main(int argc, char** argv)
{
    //TestServer();
    //TestSocket();
    //TestRotation();
    TestInterpolation(0.0, 1.0, 16);
    TestInterpolation(80.0, 40.0, 16);
    return 0;
}
