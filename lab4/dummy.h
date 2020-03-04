/**
 * @file
 * @brief Заголовочный файл, содержащий описание простейшего алгоритма
 * генерации псевдослучайных чисел
 * @date Март 2020
*/
#ifndef DUMMY_H
#define DUMMY_H

#include <cstdint>

namespace my
{

class DummyPRNG
{
public:
    using result_type = std::uint32_t;

    DummyPRNG() = delete;
    DummyPRNG(result_type seed);

    static result_type min();
    static result_type max();

    result_type operator()();

private:
    result_type m_value;
};

} // namespace my

#endif // DUMMY_H
