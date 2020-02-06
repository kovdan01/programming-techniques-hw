/**
 * @file
 * @brief Заголовочный файл, содержащий реализацию линейного поиска
 * @date Январь 2020
*/
#ifndef LINEAR_SEARCH_H
#define LINEAR_SEARCH_H

#include <iterator>
#include <functional>
#include <type_traits>
#include <vector>

namespace my
{

/**
 * Ищет в диапазоне все элементы, эквивалентные заданному
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @tparam BinaryPredicate бинарный предикат
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @param[in] are_equal объект, производящий сравнение на равенство
 * @return `std::vector`, содержащий итераторы на все элементы диапазона, эквивалентные `key`
*/
template<typename Iterator, typename Key, typename BinaryPredicate>
std::vector<Iterator> find(Iterator begin, Iterator end, const Key& key, BinaryPredicate are_equal)
{
    std::vector<Iterator> answer;
    for (; begin != end; std::advance(begin, 1))
        if (are_equal(*begin, key))
            answer.push_back(begin);
    return answer;
}

/**
 * Ищет в диапазоне все элементы, эквивалентные заданному
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @return `std::vector`, содержащий итераторы на все элементы диапазона, эквивалентные `key`
*/
template<typename Iterator, typename Key>
std::vector<Iterator> find(Iterator begin, Iterator end, const Key& key)
{
    return find(begin, end, key, std::equal_to<typename std::iterator_traits<Iterator>::value_type>());
}

} // namespace my

#endif // LINEAR_SEARCH_H
