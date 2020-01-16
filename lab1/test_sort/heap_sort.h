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
