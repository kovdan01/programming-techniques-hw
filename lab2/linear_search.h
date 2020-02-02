#ifndef LINEAR_SEARCH_H
#define LINEAR_SEARCH_H

#include <iterator>
#include <functional>
#include <type_traits>
#include <vector>

namespace my
{

template<typename Iterator, typename Key, typename BinaryPredicate>
std::vector<Iterator> find(Iterator begin, Iterator end, const Key& key, BinaryPredicate are_equal)
{
    std::vector<Iterator> answer;
    for (; begin != end; std::advance(begin, 1))
        if (are_equal(*begin, key))
            answer.push_back(begin);
    return answer;
}

template<typename Iterator, typename Key>
std::vector<Iterator> find(Iterator begin, Iterator end, const Key& key)
{
    return find(begin, end, key, std::equal_to<typename std::iterator_traits<Iterator>::value_type>());
}

} // namespace my

#endif // LINEAR_SEARCH_H
