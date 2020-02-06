/**
 * @file
 * @brief Заголовочный файл, содержащий реализацию быстрой сортировки
 * @date Январь 2020
*/
#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <iterator>
#include <stack>
#include <functional>

namespace my
{

/** Реализует быструю сортировку диапазона элементов
 * @tparam Iterator Iterator тип, удовлетворяющий C++ named requirement
 * ValueSwappable и LegacyRandomAccessIterator
 * @tparam Comparator тип, удовлетворяющий C++ named requirement Compare
 * @param[in,out] begin,end итераторы, указывающие на диапазон, который
 * требуется отсортировать
 * @param cmp компаратор: возвращает `true`, если его первый аргумент должен стоять
 * в отсортированном диапазоне строго левее второго, `false` иначе
*/
template<typename Iterator, typename Comparator>
void quick_sort(Iterator begin, Iterator end, Comparator cmp)
{
    using diff_t = typename std::iterator_traits<Iterator>::difference_type;
    auto partition = [&cmp](Iterator begin, Iterator end)
    {
        Iterator left = begin, right = end;
        diff_t size = std::distance(begin, end);
        auto pivot = *std::next(left, size / 2);
        while (std::distance(left, right) >= 0)
        {
            while (cmp(*left, pivot))
                ++left;
            while (cmp(pivot, *right))
                --right;
            if (std::distance(left, right) >= 0)
                std::iter_swap(left++, right--);
        }
        return std::make_pair(left, right);
    };

    --end;
    std::stack<std::pair<Iterator, Iterator>> operations;
    operations.emplace(begin, end);
    while (!operations.empty())
    {
        auto [left, right] = operations.top();
        operations.pop();
        auto [i, j] = partition(left, right);
        if (std::distance(left, j) > 0)
            operations.emplace(left, j);
        if (std::distance(i, right) > 0)
            operations.emplace(i, right);
    }
}

/** Реализует быструю сортировку диапазона элементов по возрастанию
 * @tparam Iterator Iterator тип, удовлетворяющий C++ named requirement
 * ValueSwappable и LegacyRandomAccessIterator
 * @param[in,out] begin,end итераторы, указывающие на диапазон, который
 * требуется отсортировать
*/
template<typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    using elem_type = typename std::iterator_traits<Iterator>::value_type;
    quick_sort(begin, end, std::less<elem_type>());
}

} // namespace my

#endif // QUICK_SORT_H
