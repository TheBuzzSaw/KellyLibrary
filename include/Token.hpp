#ifndef TOKEN_HPP_KELLY
#define TOKEN_HPP_KELLY

#include <cstdint>
#include <vector>
#include <iostream>

namespace Kelly
{
    struct Token
    {
        static constexpr int16_t InvalidUnicode = -1;
        static constexpr int16_t None = 0;
        static constexpr int16_t Space = 1;
        static constexpr int16_t Identifier = 2;
        static constexpr int16_t String = 3;
        static constexpr int16_t Number = 4;
        static constexpr int16_t Operator = 5;

        int32_t byteOffset;
        int32_t line;
        int32_t column;
        int16_t byteCount;
        int16_t type;
    };

    const char* TokenName(int16_t tokenType);
    std::vector<Token> ParseTokens(const char* source);
    std::ostream& operator<<(std::ostream& stream, Token token);
}

#endif

