#include "dummy.h"

namespace my
{

std::size_t dummy_hash(const std::string& str)
{
    std::size_t hash = 0;
    for (char c : str)
        hash += static_cast<std::uint8_t>(c);
    return hash;
}

} // namespace my
