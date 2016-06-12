#include "../include/Token.hpp"
#include "../include/Utf8CodePoint.hpp"

namespace Kelly
{
    const char* TokenName(int16_t tokenType)
    {
        switch (tokenType)
        {
            case Token::InvalidUnicode: return "invalid unicode";
            case Token::None: return "none";
            case Token::Space: return "space";
            case Token::Identifier: return "identifier";
            case Token::String: return "string";
            case Token::Number: return "number";
            case Token::Operator: return "operator";
            default: return "invalid";
        }
    }

    std::vector<Token> ParseTokens(const char* source)
    {
        std::vector<Token> tokens;
        Token token = {0, 1, 1, 0, Token::None};

        while (source[token.byteOffset])
        {
            Utf8ParseResult result =
                ParseUtf8CodePoint(source + token.byteOffset);

            if (result.parseResult < 0)
            {
                token.byteCount = 0;
                token.type = Token::InvalidUnicode;
                tokens.push_back(token);
                return tokens;
            }

            char32_t c = GetUtf32CodePoint(result.codePoint);

            //if (InRange<char32_t>(c, 'a', 'z')) ;
        }

        return tokens;
    }

    std::ostream& operator<<(std::ostream& stream, Token token)
    {
        return stream
            << "line " << token.line
            << " column " << token.column
            << " -- " << TokenName(token.type);
    }
}
