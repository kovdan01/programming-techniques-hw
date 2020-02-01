#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include <iterator>

namespace my
{

template<typename Iterator, typename Comparator>
void heap_sort(Iterator begin, Iterator end, Comparator cmp)
{
    std::ptrdiff_t size = std::distance(begin, end);
    if (size <= 1)
        return;
    std::ptrdiff_t heap_size = size;

    auto sift_down = [&cmp, &begin, &heap_size](Iterator it)
    {
        std::ptrdiff_t i = std::distance(begin, it);
        while (2 * i + 1 < heap_size)
        {
            std::ptrdiff_t left = 2 * i + 1;
            std::ptrdiff_t right = 2 * i + 2;
            std::ptrdiff_t j = left;
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

    for (std::ptrdiff_t i = 0; i < size; ++i)
    {
        std::iter_swap(begin, std::prev(end, i + 1));
        --heap_size;
        sift_down(begin);
    }
}

template<typename Iterator>
void heap_sort(Iterator begin, Iterator end)
{
    using elem_type = decltype(*begin);
    heap_sort(begin, end, [](const elem_type& lhs, const elem_type& rhs)
    {
        return lhs < rhs;
    });
}

} // namespace my

#endif // HEAP_SORT_H
