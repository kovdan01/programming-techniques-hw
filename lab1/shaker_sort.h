/**
 * @file
 * @brief Заголовочный файл, содержащий реализацию шейкер-сортировки
 * @date Январь 2020
*/
#ifndef SHAKER_SORT_H
#define SHAKER_SORT_H

#include <iterator>
#include <functional>

namespace my
{

/** Реализует шейкер-сортировку диапазона элементов
 * @tparam Iterator Iterator тип, удовлетворяющий C++ named requirement
 * ValueSwappable и LegacyRandomAccessIterator
 * @tparam Comparator тип, удовлетворяющий C++ named requirement Compare
 * @param[in,out] begin,end итераторы, указывающие на диапазон, который
 * требуется отсортировать
 * @param cmp компаратор: возвращает `true`, если его первый аргумент должен стоять
 * в отсортированном диапазоне строго левее второго, `false` иначе
*/
template<typename Iterator, typename Comparator>
void shaker_sort(Iterator begin, Iterator end, Comparator cmp)
{
    using diff_t = typename std::iterator_traits<Iterator>::difference_type;
    diff_t size = std::distance(begin, end);
    if (size <= 1)
        return;
    diff_t left = 0, right = size;
    bool exit_loop;
    auto action = [&begin, &cmp, &exit_loop](diff_t j)
    {
        auto& first = *std::next(begin, j);
        auto& second = *std::next(begin, j + 1);
        if (!cmp(first, second))
        {
            std::swap(first, second);
            exit_loop = false;
        }
    };
    for (;;)
    {
        exit_loop = true;
        for (diff_t j = left; j < right - 1; ++j)
            action(j);
        --right;
        for (diff_t j = right - 1; j >= left; --j)
            action(j);
        ++left;
        if (exit_loop)
            break;
    }
}

/** Реализует шейкер-сортировку диапазона элементов по возрастанию
 * @tparam Iterator Iterator тип, удовлетворяющий C++ named requirement
 * ValueSwappable и LegacyRandomAccessIterator
 * @param[in,out] begin,end итераторы, указывающие на диапазон, который
 * требуется отсортировать
*/
template<typename Iterator>
void shaker_sort(Iterator begin, Iterator end)
{
    using elem_type = typename std::iterator_traits<Iterator>::value_type;
    shaker_sort(begin, end, std::less<elem_type>());
}

} // namespace my

#endif // SHAKER_SORT_H
