#include "../include/Rational.hpp"
using namespace Kelly;
using namespace std;

int main(int argc, char** argv)
{
    Rational<int> a{1, 3};
    Rational<int> b{2, 3};
    a += b;
    a += 3;
    a /= 2;
    cout << a << endl;
    cout << IsNegative(Rational<int>{1, 1}) << endl;
    return 0;
}
