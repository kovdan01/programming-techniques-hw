/**
 * @file
 * @brief Заголовочный файл, содержащий описание простейшей функции хэширования строк
 * @date Февраль 2020
*/
#ifndef DUMMY_H
#define DUMMY_H

#include <cstddef>
#include <string>

namespace my
{

/**
 * Считает простейшую хэш-функцию для строки
 * @param[in] str строка, для которой необходимо посчитать хэш
 * @return значение хэша для заданной строки
 */
std::size_t dummy_hash(const std::string& str);

class DummyHash
{
public:
    std::size_t operator()(const std::string& str) const
    {
        return dummy_hash(str);
    }
};

} // namespace my

#endif // DUMMY_H
