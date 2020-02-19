#include "dummy.h"

namespace my
{

std::size_t dummy_hash(const std::string& str)
{
    std::size_t hash = 0;
    for (std::size_t i = 0; i < str.size(); ++i)
        hash += static_cast<std::uint8_t>(str[i]);
    return hash;
}

} // namespace my
