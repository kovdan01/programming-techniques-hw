#ifndef SHAKER_SORT_H
#define SHAKER_SORT_H

#include <iterator>

namespace my
{

template<typename Iterator, typename Comparator>
void shaker_sort(Iterator begin, Iterator end, Comparator cmp)
{
    std::ptrdiff_t size = std::distance(begin, end);
    if (size <= 1)
        return;
    std::ptrdiff_t left = 0, right = size;
    bool exit_loop;
    auto action = [&begin, &cmp, &exit_loop](std::ptrdiff_t j)
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
        for (std::ptrdiff_t j = left; j < right - 1; ++j)
            action(j);
        --right;
        for (std::ptrdiff_t j = right - 1; j >= left; --j)
            action(j);
        ++left;
        if (exit_loop)
            break;
    }
}

template<typename Iterator>
void shaker_sort(Iterator begin, Iterator end)
{
    using elem_type = decltype(*begin);
    shaker_sort(begin, end, [](const elem_type& lhs, const elem_type& rhs)
    {
        return lhs < rhs;
    });
}

} // namespace my

#endif // SHAKER_SORT_H
