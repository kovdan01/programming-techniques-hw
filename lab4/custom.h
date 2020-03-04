/**
 * @file
 * @brief Заголовочный файл, содержащий описание собственного алгоритма
 * генерации псевдослучайных чисел
 * @date Март 2020
*/
#ifndef CUSTOM_H
#define CUSTOM_H

#include <cstdint>

namespace my
{

class CustomPRNG
{
public:
    using result_type = std::uint32_t;

    CustomPRNG() = delete;
    CustomPRNG(result_type seed);

    static result_type min();
    static result_type max();

    result_type operator()();

private:
    result_type m_value;
};

} // namespace my

#endif // CUSTOM_H
