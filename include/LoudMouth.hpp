#ifndef LOUDMOUTH_HPP_KELLY
#define LOUDMOUTH_HPP_KELLY

#include <iostream>

namespace Kelly
{
    class LoudMouth
    {
        unsigned int _originalId;
        unsigned int _currentId;

    public:
        LoudMouth();
        LoudMouth(LoudMouth&& other) noexcept;
        LoudMouth(const LoudMouth& other);
        virtual ~LoudMouth();

        LoudMouth& operator=(LoudMouth&& other);
        LoudMouth& operator=(const LoudMouth& other);

        std::ostream& Write(std::ostream& stream) const;
    };

    std::ostream& operator<<(std::ostream& stream, const LoudMouth& lm);
}

#endif
