/**
 * @file
 * @brief Заголовочный файл, содержащий реализацию бинарного поиска
 * @date Январь 2020
*/
#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <iterator>
#include <functional>
#include <type_traits>

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

} // namespace

// lower_bound

/**
 * Ищет в отсортированном диапазоне первый элемент, не меньший заданного
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @tparam Comparator бинарный предикат
 * @tparam KeyExtractor тип, объект которого может быть вызван с аргументом типа,
 * на который указывает Iterator, и возвращающий Key
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @param[in] cmp компаратор: возвращает `true`, если его первый аргумент должен стоять
 * в отсортированном диапазоне строго левее второго, `false` иначе
 * @param[in] extractor функция, возвращающая по объекту значение, которое будет использоваться
 * при сравнении с `key`: например, `Iterator` ссылается на диапазон пар чисел,
 * отсортированных сначала по первому элементу пары, затем по второму, и требуется найти первую пару, первый
 * элемент которой не меньше `key`. В этом случае эта функция должна по паре возвращать ее первый элемент.
 * Необходимо, чтобы `extractor` сохранял свойство отсортированности диапазона
 * @return итератор на первый элемент диапазона, значение `extractor`
 * от которого не меньше `key`; `end`, если такого не нашлось
*/
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

/**
 * Ищет в отсортированном диапазоне первый элемент, не меньший заданного
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @tparam Comparator бинарный предикат
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @param[in] cmp компаратор: возвращает `true`, если его первый аргумент должен стоять
 * в отсортированном диапазоне строго левее второго, `false` иначе
 * @return итератор на первый элемент диапазона, значение которого не меньше `key`;
 * `end`, если такого не нашлось
*/
template<typename Iterator, typename Key, typename Comparator>
auto lower_bound(Iterator begin, Iterator end, const Key& key, Comparator cmp) ->
    check_comparator<Iterator, Comparator>
{
    return my::lower_bound(begin, end, key, cmp, trivial_extractor<Iterator>);
}

/**
 * Ищет в отсортированном диапазоне первый элемент, не меньший заданного
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @tparam KeyExtractor тип, объект которого может быть вызван с аргументом типа,
 * на который указывает Iterator, и возвращающий Key
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @param[in] extractor функция, возвращающая по объекту значение, которое будет использоваться
 * при сравнении с `key`: например, `Iterator` ссылается на диапазон пар чисел,
 * отсортированных сначала по первому элементу пары, затем по второму, и требуется найти первую пару, первый
 * элемент которой не меньше `key`. В этом случае эта функция должна по паре возвращать ее первый элемент.
 * Необходимо, чтобы `extractor` сохранял свойство отсортированности диапазона
 * @return итератор на первый элемент диапазона, значение `extractor`
 * от которого не меньше `key`; `end`, если такого не нашлось
*/
template<typename Iterator, typename Key, typename KeyExtractor>
auto lower_bound(Iterator begin, Iterator end, const Key& key, KeyExtractor extractor) ->
    check_key_extractor<Iterator, Key, KeyExtractor>
{
    return my::lower_bound(begin, end, key, std::less<Key>(), extractor);
}

/**
 * Ищет в отсортированном диапазоне первый элемент, не меньший заданного
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @return итератор на первый элемент диапазона, значение которого не меньше `key`;
 * `end`, если такого не нашлось
*/
template<typename Iterator, typename Key>
Iterator lower_bound(Iterator begin, Iterator end, const Key& key)
{
    return my::lower_bound(begin, end, key, std::less<elem_type<Iterator>>(), trivial_extractor<Iterator>);
}

// upper_bound

/**
 * Ищет в отсортированном диапазоне первый элемент, строго больший заданного
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @tparam Comparator бинарный предикат
 * @tparam KeyExtractor тип, объект которого может быть вызван с аргументом типа,
 * на который указывает Iterator, и возвращающий Key
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @param[in] cmp компаратор: возвращает `true`, если его первый аргумент должен стоять
 * в отсортированном диапазоне строго левее второго, `false` иначе
 * @param[in] extractor функция, возвращающая по объекту значение, которое будет использоваться
 * при сравнении с `key`: например, `Iterator` ссылается на диапазон пар чисел,
 * отсортированных сначала по первому элементу пары, затем по второму, и требуется найти первую пару, первый
 * элемент которой строго больше `key`. В этом случае эта функция должна по паре возвращать ее первый элемент.
 * Необходимо, чтобы `extractor` сохранял свойство отсортированности диапазона
 * @return итератор на первый элемент диапазона, значение `extractor`
 * от которого строго больше `key`; `end`, если такого не нашлось
*/
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

/**
 * Ищет в отсортированном диапазоне первый элемент, строго больший заданного
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @tparam Comparator бинарный предикат
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @param[in] cmp компаратор: возвращает `true`, если его первый аргумент должен стоять
 * в отсортированном диапазоне строго левее второго, `false` иначе
 * @return итератор на первый элемент диапазона, значение которого строго больше `key`;
 * `end`, если такого не нашлось
*/
template<typename Iterator, typename Key, typename Comparator>
auto upper_bound(Iterator begin, Iterator end, const Key& key, Comparator cmp) ->
    check_comparator<Iterator, Comparator>
{
    return my::upper_bound(begin, end, key, cmp, trivial_extractor<Iterator>);
}

/**
 * Ищет в отсортированном диапазоне первый элемент, строго больший заданного
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @tparam KeyExtractor тип, объект которого может быть вызван с аргументом типа,
 * на который указывает Iterator, и возвращающий Key
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @param[in] extractor функция, возвращающая по объекту значение, которое будет использоваться
 * при сравнении с `key`: например, `Iterator` ссылается на диапазон пар чисел,
 * отсортированных сначала по первому элементу пары, затем по второму, и требуется найти первую пару, первый
 * элемент которой строго больше `key`. В этом случае эта функция должна по паре возвращать ее первый элемент.
 * Необходимо, чтобы `extractor` сохранял свойство отсортированности диапазона
 * @return итератор на первый элемент диапазона, значение `extractor`
 * от которого строго больше `key`; `end`, если такого не нашлось
*/
template<typename Iterator, typename Key, typename KeyExtractor>
auto upper_bound(Iterator begin, Iterator end, const Key& key, KeyExtractor extractor) ->
    check_key_extractor<Iterator, Key, KeyExtractor>
{
    return my::upper_bound(begin, end, key, std::less<Key>(), extractor);
}

/**
 * Ищет в отсортированном диапазоне первый элемент, строго больший заданного
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @return итератор на первый элемент диапазона, значение которого строго больше `key`;
 * `end`, если такого не нашлось
*/
template<typename Iterator, typename Key>
Iterator upper_bound(Iterator begin, Iterator end, const Key& key)
{
    return my::upper_bound(begin, end, key, std::less<elem_type<Iterator>>(), trivial_extractor<Iterator>);
}

// equal_range

/**
 * Ищет в отсортированном диапазоне отрезок с элементами, эквивалентными заданному
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @tparam Comparator бинарный предикат
 * @tparam KeyExtractor тип, объект которого может быть вызван с аргументом типа,
 * на который указывает Iterator, и возвращающий Key
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @param[in] cmp компаратор: возвращает `true`, если его первый аргумент должен стоять
 * в отсортированном диапазоне строго левее второго, `false` иначе
 * @param[in] extractor функция, возвращающая по объекту значение, которое будет использоваться
 * при сравнении с `key`: например, `Iterator` ссылается на диапазон пар чисел,
 * отсортированных сначала по первому элементу пары, затем по второму, и требуется найти диапазон пар, первый
 * элемент которых равен `key`. В этом случае эта функция должна по паре возвращать ее первый элемент.
 * Необходимо, чтобы `extractor` сохранял свойство отсортированности диапазона
 * @return пара итераторов, первый элемент которой указывает на первый элемент найденного отрезка,
 * второй - на элемент, следующий за последним в отрезке эквивалентных; если нужного отрезка не
 * нашлось, оба элемента пары будут равны `end`
*/
template<typename Iterator, typename Key, typename Comparator, typename KeyExtractor>
std::pair<Iterator, Iterator> equal_range(Iterator begin, Iterator end, const Key& key, Comparator cmp, KeyExtractor extractor)
{
    return {my::lower_bound(begin, end, key, cmp, extractor),
            my::upper_bound(begin, end, key, cmp, extractor)};
}

/**
 * Ищет в отсортированном диапазоне отрезок с элементами, эквивалентными заданному
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @tparam Comparator бинарный предикат
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @param[in] cmp компаратор: возвращает `true`, если его первый аргумент должен стоять
 * в отсортированном диапазоне строго левее второго, `false` иначе
 * @return пара итераторов, первый элемент которой указывает на первый элемент найденного отрезка,
 * второй - на элемент, следующий за последним в отрезке эквивалентных; если нужного отрезка не
 * нашлось, оба элемента пары будут равны `end`
*/
template<typename Iterator, typename Key, typename Comparator>
std::pair<Iterator, Iterator> equal_range(Iterator begin, Iterator end, const Key& key, Comparator cmp)
{
    return {my::lower_bound(begin, end, key, cmp),
            my::upper_bound(begin, end, key, cmp)};
}

/**
 * Ищет в отсортированном диапазоне отрезок с элементами, эквивалентными заданному
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @tparam KeyExtractor тип, объект которого может быть вызван с аргументом типа,
 * на который указывает Iterator, и возвращающий Key
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @param[in] extractor функция, возвращающая по объекту значение, которое будет использоваться
 * при сравнении с `key`: например, `Iterator` ссылается на диапазон пар чисел,
 * отсортированных сначала по первому элементу пары, затем по второму, и требуется найти диапазон пар, первый
 * элемент которых равен `key`. В этом случае эта функция должна по паре возвращать ее первый элемент.
 * Необходимо, чтобы `extractor` сохранял свойство отсортированности диапазона
 * @return пара итераторов, первый элемент которой указывает на первый элемент найденного отрезка,
 * второй - на элемент, следующий за последним в отрезке эквивалентных; если нужного отрезка не
 * нашлось, оба элемента пары будут равны `end`
*/
template<typename Iterator, typename Key, typename KeyExtractor>
std::pair<Iterator, Iterator> upper_bound(Iterator begin, Iterator end, const Key& key, KeyExtractor extractor)
{
    return {my::lower_bound(begin, end, key, extractor),
            my::upper_bound(begin, end, key, extractor)};
}

/**
 * Ищет в отсортированном диапазоне отрезок с элементами, эквивалентными заданному
 * @tparam Iterator тип, удовлетворяющий C++ named requirement LegacyForwardIterator
 * @tparam Key тип элемента, с которым будет производиться сравнение
 * @param[in] begin,end итераторы, указывающие на отсортированный диапазон, в котором будет производиться поиск
 * @param[in] key элемент, по которому производится поиск
 * @return пара итераторов, первый элемент которой указывает на первый элемент найденного отрезка,
 * второй - на элемент, следующий за последним в отрезке эквивалентных; если нужного отрезка не
 * нашлось, оба элемента пары будут равны `end`
*/
template<typename Iterator, typename Key, typename Comparator>
std::pair<Iterator, Iterator> equal_range(Iterator begin, Iterator end, const Key& key)
{
    return {lower_bound(begin, end, key),
            upper_bound(begin, end, key)};
}

} // namespace my

#endif // BINARY_SEARCH_H
