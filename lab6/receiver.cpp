#include "receiver.h"

#include <vector>
#include <cstdint>
#include <iostream>

namespace my
{

void Receiver::extend_if_needed(std::size_t size)
{
    if (m_tape.size() < size)
        m_tape.resize(size, 0);
}

Receiver::Receiver()
    : m_tape(DEFAULT_SIZE, 0)
{
}

void Receiver::set_element(std::size_t index, std::uint32_t element)
{
    extend_if_needed(index + 1);
    m_tape[index] = element;
}

std::uint32_t Receiver::get_element(std::size_t index) const
{
    if (index < m_tape.size())
        return m_tape[index];
    return 0;
}

void Receiver::print() const
{
    for (std::uint32_t element : m_tape)
        std::cout << element << ' ';
    std::cout << '\n';
}

} // namespace my
