#include "dummy.h"
#include <limits>

namespace my
{

DummyPRNG::DummyPRNG(DummyPRNG::result_type seed)
    : m_value(seed)
{
}

DummyPRNG::result_type DummyPRNG::operator()()
{
    m_value *= m_value + 1;
    return m_value;
}

DummyPRNG::result_type DummyPRNG::min()
{
    return std::numeric_limits<DummyPRNG::result_type>::min();
}

DummyPRNG::result_type DummyPRNG::max()
{
    return std::numeric_limits<DummyPRNG::result_type>::max();
}

} // namespace my
