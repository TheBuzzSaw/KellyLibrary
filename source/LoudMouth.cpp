#include "../include/LoudMouth.hpp"
using namespace std;

namespace Kelly
{
    static unsigned int lastId = 0;

    LoudMouth::LoudMouth()
    {
        _originalId = ++lastId;
        _currentId = _originalId;
        _junk = 0;

        cout << "construct " << *this << endl;
    }

    LoudMouth::LoudMouth(LoudMouth&& other) noexcept
    {
        _originalId = ++lastId;
        _currentId = other._currentId;
        _junk = other._junk;

        cout << "move construct " << *this << " from " << other << endl;

        other._currentId = 0;
    }

    LoudMouth::LoudMouth(const LoudMouth& other)
    {
        _originalId = ++lastId;
        _currentId = other._currentId;
        _junk = other._junk;

        cout << "copy construct " << *this << " from " << other << endl;
    }

    LoudMouth::~LoudMouth()
    {
        cout << "destruct " << *this << endl;
    }

    LoudMouth& LoudMouth::operator=(LoudMouth&& other)
    {
        cout << *this << " move assign " << other << endl;

        _currentId = other._currentId;
        other._currentId = 0;
        return *this;
    }

    LoudMouth& LoudMouth::operator=(const LoudMouth& other)
    {
        cout << *this << " copy assign " << other << endl;

        _currentId = other._currentId;
        return *this;
    }

    ostream& LoudMouth::Write(ostream& stream) const
    {
        return stream << _currentId << '(' << _originalId << ')';
    }

    ostream& operator<<(ostream& stream, const LoudMouth& lm)
    {
        return lm.Write(stream);
    }
}
