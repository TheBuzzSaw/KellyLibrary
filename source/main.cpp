#include "../include/StructureOfArrays.hpp"
#include <string>
#include <cstring>
#include <iostream>
using namespace Kelly;
using namespace std;

void TestStructureOfArrays()
{
    StructureOfArrays<4> soa{4, 2, 2, 8};
    soa.Resize(3);
    cout << "capacity: " << soa.Capacity() << endl;
    cout << "count: " << soa.Count() << endl;
    auto values = soa.GetArray<int64_t>(3);
    for (auto& value : values) value = 555;
    for (auto value : values) cout << ' ' << value;
    cout << '\n';

    soa.Resize(55);
    soa.Resize(3);
    cout << "capacity: " << soa.Capacity() << endl;
    cout << "count: " << soa.Count() << endl;
    values = soa.GetArray<int64_t>(3);
    for (auto value : values) cout << ' ' << value;
}

int main(int argc, char** argv)
{
    TestStructureOfArrays();
    return 0;
}
