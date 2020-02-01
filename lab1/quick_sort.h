#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <iterator>
#include <stack>

namespace my
{

template<typename Iterator, typename Comparator>
void quick_sort(Iterator begin, Iterator end, Comparator cmp)
{
    auto partition = [&cmp](Iterator begin, Iterator end)
    {
        Iterator left = begin, right = end;
        std::ptrdiff_t size = std::distance(begin, end);
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

template<typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    using elem_type = decltype(*begin);
    quick_sort(begin, end, [](const elem_type& lhs, const elem_type& rhs)
    {
        return lhs < rhs;
    });
}

} // namespace my

#endif // QUICK_SORT_H
