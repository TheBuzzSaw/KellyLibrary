#include "../include/Rational.hpp"
using namespace Kelly;
using namespace std;

void TestOperators(Rational<int> r1, Rational<int> r2)
{
    cout << "compare " << r1 << " to " << r2;

    cout
        << "\n== " << (r1 == r2)
        << "\n!= " << (r1 != r2)
        << "\n< " << (r1 < r2)
        << "\n> " << (r1 > r2)
        << "\n<= " << (r1 <= r2)
        << "\n>= " << (r1 >= r2)
        << "\n/ " << (r1 / r2)
        << endl;
}

int main(int argc, char** argv)
{
    TestOperators({1, 3}, {-1, -6});
    return 0;
}
