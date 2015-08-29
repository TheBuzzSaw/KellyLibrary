#include "../include/FileTools.hpp"
#include <fstream>
#include <sstream>

namespace Kelly
{
    std::string ReadAllText(const char* path)
    {
        std::ostringstream oss;

        std::ifstream fin(path, std::ifstream::binary);

        if (fin)
        {
            oss << fin.rdbuf();
            fin.close();
        }

        return oss.str();
    }
}
