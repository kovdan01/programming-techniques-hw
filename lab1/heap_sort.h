/**
 * @file
 * @brief Заголовочный файл, содержащий реализацию пирамидальной сортировки
 * @date Январь 2020
*/
#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include <iterator>
#include <functional>

namespace my
{

/** Реализует пирамидальную сортировку диапазона элементов
 * @tparam Iterator Iterator тип, удовлетворяющий C++ named requirement
 * ValueSwappable и LegacyRandomAccessIterator
 * @tparam Comparator тип, удовлетворяющий C++ named requirement Compare
 * @param[in,out] begin,end итераторы, указывающие на диапазон, который
 * требуется отсортировать
 * @param cmp компаратор: возвращает `true`, если его первый аргумент должен стоять
 * в отсортированном диапазоне строго левее второго, `false` иначе
*/
template<typename Iterator, typename Comparator>
void heap_sort(Iterator begin, Iterator end, Comparator cmp)
{
    using diff_t = typename std::iterator_traits<Iterator>::difference_type;
    diff_t size = std::distance(begin, end);
    if (size <= 1)
        return;
    diff_t heap_size = size;

    auto sift_down = [&cmp, &begin, &heap_size](Iterator it)
    {
        diff_t i = std::distance(begin, it);
        while (2 * i + 1 < heap_size)
        {
            diff_t left = 2 * i + 1;
            diff_t right = 2 * i + 2;
            diff_t j = left;
            if (right < heap_size && cmp(*std::next(begin, left), *std::next(begin, right)))
                j = right;
            Iterator it = std::next(begin, i);
            Iterator jt = std::next(begin, j);
            if (!cmp(*it, *jt))
                break;
            std::iter_swap(it, jt);
            i = std::distance(begin, jt);
        }
    };

    for (Iterator it = std::next(begin, size / 2); it != std::prev(begin); --it)
        sift_down(it);

    for (diff_t i = 0; i < size; ++i)
    {
        std::iter_swap(begin, std::prev(end, i + 1));
        --heap_size;
        sift_down(begin);
    }
}

/** Реализует пирамидальную сортировку диапазона элементов по возрастанию
 * @tparam Iterator Iterator тип, удовлетворяющий C++ named requirement
 * ValueSwappable и LegacyRandomAccessIterator
 * @param[in,out] begin,end итераторы, указывающие на диапазон, который
 * требуется отсортировать
*/
template<typename Iterator>
void heap_sort(Iterator begin, Iterator end)
{
    using elem_type = typename std::iterator_traits<Iterator>::value_type;
    heap_sort(begin, end, std::less<elem_type>());
}

} // namespace my

#endif // HEAP_SORT_H
