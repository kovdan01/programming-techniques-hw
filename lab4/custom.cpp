#include "custom.h"
#include <limits>

namespace my
{

CustomPRNG::CustomPRNG(CustomPRNG::result_type seed)
    : m_value(seed)
{
}

CustomPRNG::result_type CustomPRNG::operator()()
{
    std::uint32_t old = m_value;
    m_value <<= 7;
    m_value += old ^ 0x9908b0df;
    return m_value;
}

CustomPRNG::result_type CustomPRNG::min()
{
    return std::numeric_limits<CustomPRNG::result_type>::min();
}

CustomPRNG::result_type CustomPRNG::max()
{
    return std::numeric_limits<CustomPRNG::result_type>::max();
}

} // namespace my
