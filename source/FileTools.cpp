#include "../include/FileTools.hpp"
#include <fstream>
#include <sstream>

namespace Kelly
{
    std::string ReadAllText(const char* path)
    {
        std::ifstream fin(path, std::ifstream::binary);

        if (fin)
        {
            std::ostringstream oss;
            oss << fin.rdbuf();
            fin.close();
            return oss.str();
        }

        return std::string();
    }
}
