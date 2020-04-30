#ifndef RECEIVER_H
#define RECEIVER_H

#include <vector>
#include <cstdint>

namespace my
{

class Receiver
{
public:
    static inline constexpr std::size_t DEFAULT_SIZE = 16;

    Receiver();
    void set_element(std::size_t index, std::uint32_t element);
    std::uint32_t get_element(std::size_t index) const;
    void print() const;

private:
    std::vector<std::uint32_t> m_tape;

    void extend_if_needed(std::size_t size);
};

} // namespace my

#endif // RECEIVER_H
