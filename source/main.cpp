#include "../include/Network/Endpoint32Query.hpp"
#include "../include/Network/TcpListener.hpp"
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

    query = Endpoint32Query("google.com", "80");
    auto ii = query.begin();
    if (ii == query.end())
    {
        cerr << "failed to query google.com\n";
        return;
    }
    auto endpoint = *ii;

    TcpSocket socket(endpoint);
    if (!socket.IsOpen())
    {
        cerr << "failed to open socket\n";
        return;
    }

    char buffer[] =
        "GET / HTTP/1.1\r\n"
        "Host: www.google.com\r\n"
        "Connection: close\r\n"
        "\r\n";
    View<uint8_t> view = { (uint8_t*)buffer, sizeof(buffer) };
    socket.Send(view);

    ptrdiff_t n;
    while ((n = socket.Receive(view)) > 0)
    {
        cout.write((const char*)view.data, n);
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
    auto socket = listener.Accept();
    if (socket.IsOpen())
    {
        cout << "connection established from " << socket.Endpoint() << endl;

        char buffer[1024];
        View<uint8_t> view = { (uint8_t*)buffer, sizeof(buffer) };

        ptrdiff_t n;
        while ((n = socket.Receive(view)) > 0)
        {
            cout.write(buffer, n);
            socket.SetBlocking(false);
        }

        auto response =
            "HTTP/1.1 200 OK\r\n"
            "Connection: close\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "\r\n"
            "<html><head><title>XPG Server</title></head>"
            "<body><p>Hello, World! Text is da bomb.</p></body>"
            "</html>"
            ;

        strcpy(buffer, response);
        view.n = strlen(buffer);

        socket.Send(view);
    }
    else
    {
        cerr << "failed to accept\n";
        return;
    }
}

int main(int argc, char** argv)
{
    TestServer();
    return 0;
}
