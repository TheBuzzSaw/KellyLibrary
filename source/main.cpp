#include "../include/Rational.hpp"
using namespace Kelly;
using namespace std;

int main(int argc, char** argv)
{
    Rational<int> a{1, 3};
    Rational<int> b{2, 3};
    cout << (a - b) << endl;
    cout << IsNegative(Rational<int>{1, 1}) << endl;
    return 0;
}
