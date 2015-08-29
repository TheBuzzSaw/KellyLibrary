#include "../include/FileTools.hpp"
#include <fstream>
#include <iostream>
using namespace Kelly;
using namespace std;

void Dump(const char* path)
{
    string content = ReadAllText(path);

    //cout << "Processing " << path << endl;
    cout << "<ul>\n";

    char VideoId[] = "\"videoId\": ";
    char Title[] = "\"title\": ";
    auto n = content.find(VideoId);
    while (n != string::npos)
    {
        cout << "<li><a href=\"https://www.youtube.com/watch?v=";
        cout.write(content.data() + n + sizeof(VideoId), 11) << "\">";

        auto nn = content.find(Title, n + 1);
        if (nn != string::npos)
        {
            for (auto i = content.data() + nn + sizeof(Title); *i != '"'; ++i)
            {
                switch (*i)
                {
                    case '\\': ++i; cout << '"'; break;
                    case '&': cout << "&amp;"; break;
                    default: cout << *i; break;
                }
            }
        }

        cout << "</a></li>\n";

        n = content.find(VideoId, n + sizeof(VideoId));
    }

    cout << "</ul>\n";
}

int main(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i) Dump(argv[i]);

    return 0;
}
