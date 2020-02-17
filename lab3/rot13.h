/**
 * @file
 * @brief Заголовочный файл, содержащий описание функции хэширования строк rot13
 * @date Февраль 2020
*/
#ifndef ROT13_H
#define ROT13_H

#include <cstddef>
#include <string>

namespace my
{

/**
 * Считает хэш-функцию rot13 для строки
 * @param[in] str строка, для которой необходимо посчитать хэш
 * @return значение хэша rot13 для заданной строки
 */
std::size_t rot13_hash(const std::string& str);

/**
 * Улучшенная версия rot13 для работы с 64-битными хэшами
 */
std::size_t rot19_hash(const std::string& str);

class Rot13Hash
{
public:
    std::size_t operator()(const std::string& str) const
    {
        return rot13_hash(str);
    }
};

class Rot19Hash
{
public:
    std::size_t operator()(const std::string& str) const
    {
        return rot19_hash(str);
    }
};

} // namespace my

#endif // ROT13_H
