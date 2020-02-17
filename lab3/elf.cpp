#include "elf.h"

namespace my
{

std::size_t elf_hash(const std::string& str)
{
    std::size_t h = 0, high;
    for (char c : str)
    {
        h = (h << 4) + static_cast<std::size_t>(c++);
        if ((high = (h & 0xF0000000)))
            h ^= high >> 24;
        h &= ~high;
    }
    return h;
}

} // namespace my
