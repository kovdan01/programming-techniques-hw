/**
 * @file
 * @brief Заголовочный файл, содержащий реализацию функции хэширования строк ELF
 * @date Февраль 2020
*/
#ifndef ELF_H
#define ELF_H

#include <cstddef>
#include <string>

namespace my
{

/**
 * Считает хэш-функцию elf для строки
 * @param[in] str строка, для которой необходимо посчитать хэш
 * @return значение хэша elf для заданной строки
 */
std::size_t elf_hash(const std::string& str);

class ElfHash
{
public:
    std::size_t operator()(const std::string& str) const
    {
        return elf_hash(str);
    }
};

} // namespace my

#endif // ELF_H
