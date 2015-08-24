#ifndef LOUDMOUTH_HPP_KELLY
#define LOUDMOUTH_HPP_KELLY

#include <iostream>

namespace Kelly
{
    class LoudMouth
    {
        unsigned int _originalId;
        unsigned int _currentId;
        unsigned int _junk;

    public:
        LoudMouth();
        LoudMouth(LoudMouth&& other) noexcept;
        LoudMouth(const LoudMouth& other);
        virtual ~LoudMouth();

        LoudMouth& operator=(LoudMouth&& other);
        LoudMouth& operator=(const LoudMouth& other);

        inline void Poke() { _junk ^= 0xdeadbeef; }

        std::ostream& Write(std::ostream& stream) const;
    };

    std::ostream& operator<<(std::ostream& stream, const LoudMouth& lm);
}

#endif
