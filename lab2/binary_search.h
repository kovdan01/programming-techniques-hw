#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <iterator>

namespace my
{

template<typename Iterator, typename Key, typename Comparator, typename KeyExtractor>
Iterator lower_bound(Iterator begin, Iterator end, Key key, Comparator cmp, KeyExtractor extractor)
{
    Iterator current;
    std::ptrdiff_t length = std::distance(begin, end), half;
    while (length > 0)
    {
        current = begin;
        half = length / 2;
        std::advance(current, half);
        if (cmp(extractor(*current), key))
        {
            begin = ++current;
            length -= half + 1;
        }
        else
        {
            length = half;
        }
    }
    return begin;
}

template<typename Iterator, typename Key, typename Comparator>
Iterator lower_bound(Iterator begin, Iterator end, Key key, Comparator cmp)
{
    using elem_type = decltype (*begin);
    return lower_bound(begin, end, key, cmp, [](const elem_type& elem){ return elem; });
}

template<typename Iterator, typename Key>
Iterator lower_bound(Iterator begin, Iterator end, Key key)
{
    using elem_type = decltype (*begin);
    return lower_bound(begin, end, key, [](const elem_type& lhs, const elem_type& rhs){ return lhs < rhs; });
}

template<typename Iterator, typename Key, typename Comparator, typename KeyExtractor>
Iterator upper_bound(Iterator begin, Iterator end, Key key, Comparator cmp, KeyExtractor extractor)
{
    Iterator current;
    std::ptrdiff_t length = std::distance(begin, end), half;
    while (length > 0)
    {
        current = begin;
        half = length / 2;
        std::advance(current, half);
        if (!cmp(key, extractor(*current)))
        {
            begin = ++current;
            length -= half + 1;
        }
        else
        {
            length = half;
        }
    }
    return begin;
}

template<typename Iterator, typename Key, typename Comparator>
Iterator upper_bound(Iterator begin, Iterator end, Key key, Comparator cmp)
{
    using elem_type = decltype (*begin);
    return upper_bound(begin, end, key, cmp, [](const elem_type& elem){ return elem; });
}

template<typename Iterator, typename Key>
Iterator upper_bound(Iterator begin, Iterator end, Key key)
{
    using elem_type = decltype (*begin);
    return upper_bound(begin, end, key, [](const elem_type& lhs, const elem_type& rhs){ return lhs < rhs; });
}

template<typename Iterator, typename Key, typename Comparator, typename KeyExtractor>
std::pair<Iterator, Iterator> equal_range(Iterator begin, Iterator end, Key key, Comparator cmp, KeyExtractor extractor)
{
    return {lower_bound(begin, end, key, cmp, extractor),
            upper_bound(begin, end, key, cmp, extractor)};
}

template<typename Iterator, typename Key, typename Comparator>
std::pair<Iterator, Iterator> equal_range(Iterator begin, Iterator end, Key key, Comparator cmp)
{
    return {lower_bound(begin, end, key, cmp),
            upper_bound(begin, end, key, cmp)};
}

template<typename Iterator, typename Key, typename Comparator>
std::pair<Iterator, Iterator> equal_range(Iterator begin, Iterator end, Key key)
{
    return {lower_bound(begin, end, key),
            upper_bound(begin, end, key)};
}

} // namespace my

#endif // BINARY_SEARCH_H
