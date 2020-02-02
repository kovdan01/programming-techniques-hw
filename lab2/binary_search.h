#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <iterator>
#include <functional>
#include <type_traits>
#include <utility>

namespace my
{

namespace
{

// helper aliases
template<typename Iterator>
using elem_type = typename std::iterator_traits<Iterator>::value_type;

template<typename Iterator>
using diff_t = typename std::iterator_traits<Iterator>::difference_type;

// Iterator if functions can be invoked in a right way, nothing otherwise
template<typename Iterator, typename Key, typename KeyExtractor>
using check_key_extractor = typename std::enable_if<
    std::is_invocable_r<const Key&, KeyExtractor, const elem_type<Iterator>&>::value,
    Iterator>::type;

template<typename Iterator, typename Comparator>
using check_comparator = typename std::enable_if<
    std::is_invocable_r<bool, Comparator, const elem_type<Iterator>&, const elem_type<Iterator>&>::value,
    Iterator>::type;

template<typename Iterator, typename Key, typename Comparator, typename KeyExtractor>
using check_comparator_and_key_extractor = typename std::enable_if<
        std::is_invocable_r<const Key&, KeyExtractor, const elem_type<Iterator>&>::value &&
        std::is_invocable_r<bool, Comparator, const Key&, const Key&>::value,
    Iterator>::type;

template<typename Iterator>
const elem_type<Iterator>& trivial_extractor(const elem_type<Iterator>& elem)
{
    return elem;
}

}

// lower_bound

template<typename Iterator, typename Key, typename Comparator, typename KeyExtractor>
auto lower_bound(Iterator begin, Iterator end, const Key& key, Comparator cmp, KeyExtractor extractor) ->
    check_comparator_and_key_extractor<Iterator, Key, Comparator, KeyExtractor>
{
    diff_t<Iterator> length = std::distance(begin, end), half;
    while (length > 0)
    {
        Iterator current = begin;
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
auto lower_bound(Iterator begin, Iterator end, const Key& key, Comparator cmp) ->
    check_comparator<Iterator, Comparator>
{
    return my::lower_bound(begin, end, key, cmp, trivial_extractor<Iterator>);
}

template<typename Iterator, typename Key, typename KeyExtractor>
auto lower_bound(Iterator begin, Iterator end, const Key& key, KeyExtractor extractor) ->
    check_key_extractor<Iterator, Key, KeyExtractor>
{
    return my::lower_bound(begin, end, key, std::less<Key>(), extractor);
}

template<typename Iterator, typename Key>
Iterator lower_bound(Iterator begin, Iterator end, const Key& key)
{
    return my::lower_bound(begin, end, key, std::less<elem_type<Iterator>>(), trivial_extractor<Iterator>);
}

// upper_bound

template<typename Iterator, typename Key, typename Comparator, typename KeyExtractor>
auto upper_bound(Iterator begin, Iterator end, const Key& key, Comparator cmp, KeyExtractor extractor) ->
    check_comparator_and_key_extractor<Iterator, Key, Comparator, KeyExtractor>
{
    diff_t<Iterator> length = std::distance(begin, end), half;
    while (length > 0)
    {
        Iterator current;
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
auto upper_bound(Iterator begin, Iterator end, const Key& key, Comparator cmp) ->
    check_comparator<Iterator, Comparator>
{
    return my::upper_bound(begin, end, key, cmp, trivial_extractor<Iterator>);
}

template<typename Iterator, typename Key, typename KeyExtractor>
auto upper_bound(Iterator begin, Iterator end, const Key& key, KeyExtractor extractor) ->
    check_key_extractor<Iterator, Key, KeyExtractor>
{
    return my::upper_bound(begin, end, key, std::less<Key>(), extractor);
}

template<typename Iterator, typename Key>
Iterator upper_bound(Iterator begin, Iterator end, const Key& key)
{
    return my::upper_bound(begin, end, key, std::less<elem_type<Iterator>>(), trivial_extractor<Iterator>);
}

// equal_range

template<typename Iterator, typename Key, typename Comparator, typename KeyExtractor>
std::pair<Iterator, Iterator> equal_range(Iterator begin, Iterator end, const Key& key, Comparator cmp, KeyExtractor extractor)
{
    return {my::lower_bound(begin, end, key, cmp, extractor),
            my::upper_bound(begin, end, key, cmp, extractor)};
}

template<typename Iterator, typename Key, typename Comparator>
std::pair<Iterator, Iterator> equal_range(Iterator begin, Iterator end, const Key& key, Comparator cmp)
{
    return {my::lower_bound(begin, end, key, cmp),
            my::upper_bound(begin, end, key, cmp)};
}

template<typename Iterator, typename Key, typename KeyExtractor>
std::pair<Iterator, Iterator> upper_bound(Iterator begin, Iterator end, const Key& key, KeyExtractor extractor)
{
    return {my::lower_bound(begin, end, key, extractor),
            my::upper_bound(begin, end, key, extractor)};
}

template<typename Iterator, typename Key, typename Comparator>
std::pair<Iterator, Iterator> equal_range(Iterator begin, Iterator end, const Key& key)
{
    return {lower_bound(begin, end, key),
            upper_bound(begin, end, key)};
}

} // namespace my

#endif // BINARY_SEARCH_H
