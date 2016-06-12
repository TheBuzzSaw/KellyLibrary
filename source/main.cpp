#include <iostream>
#include <iomanip>
#include "../include/Matrix4x4.hpp"
using namespace Kelly;
using namespace std;

int main(int argc, char** argv)
{
    (void)argc, (void)argv;

    auto m = Identity4x4<int>();
    int i = 0;
    for (int& n : m.values) n = i++;

    cout << m << '\n' << (m * m) << endl;

    return 0;
}
