#include "../include/Region.hpp"
#include "../include/StructureOfArrays.hpp"
#include <string>
#include <cstring>
#include <iostream>
#include <random>
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

void TestRegion()
{
    Region region;

    mt19937_64 mt;
    uniform_int_distribution<int> distribution(1, 64);

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < (1 << 12); ++j)
        {
            int count = distribution(mt);
            auto values = new (region) int[count];

            for (int k = 0; k < count; ++k)
                values[k] = distribution(mt);
        }

        region.DebugDump();
    }
}

void TestRoot()
{
    for (int i = 0; i < 26; ++i)
        cout << ' ' << i << '(' << IntRoot(i) << ')';

    cout << endl;
}

int main(int argc, char** argv)
{
    TestRoot();
    return 0;
}
