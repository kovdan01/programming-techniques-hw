#include "rot13.h"

namespace my
{

std::size_t rot13_hash(const std::string& str)
{
    std::size_t hash = 0;
    for (char c : str)
    {
        hash += static_cast<std::uint8_t>(c);
        hash -= (hash << 13) | (hash >> 19);
    }
    return hash;
}

std::size_t rot19_hash(const std::string& str)
{
    std::size_t hash = 0;
    for (char c : str)
    {
        hash += static_cast<std::uint8_t>(c);
        hash -= (hash << 19) | (hash >> 43);
    }
    return hash;
}

} // namespace my
