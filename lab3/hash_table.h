#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <functional>
#include <cstddef>
#include <vector>
#include <forward_list>
#include <list>
#include <utility>
#include <optional>
#include <cmath>
#include <cassert>

#ifndef NDEBUG
#include <iostream>
#endif

namespace my
{

/**
 * Простейшая реализация хэш-таблицы (аналога `std::unordered_multimap`)
 * @tparam Key тип ключа данных
 * @tparam T тип хранимого значения
 * @tparam Hash тип, являющийся default-constructible и объект которого можно вызвать
 * с параметром типа `Key` и получить хэш в виде `std::size_t`
 */
template <typename Key, typename T, typename Hash = std::hash<Key>>
class HashTable
{
public:
    HashTable()
        : m_hash({})
        , m_data(std::vector<Bucket>(17))
    {}
    HashTable(const HashTable&) = default;
    HashTable& operator=(const HashTable&) = default;
    HashTable(HashTable&&) noexcept = default;
    HashTable& operator=(HashTable&&) noexcept = default;

    /**
     * Добавляет в хэш-таблицу элемент с заданным ключом и значением
     * @param[in] key ключ добавляемого элемента
     * @param[in] value значение добавляемого элемента
     */
    void emplace(const Key& key, T value)
    {
        emplace(key, std::forward_list<T>{ std::move(value) });
    }

    /**
     * Добавляет в хэш-таблицу список элементов элемент с заданным ключом
     * @param[in] key ключ добавляемых элемента
     * @param[in] values список, содержащий значения добавляемых элементов
     */
    void emplace(const Key& key, std::forward_list<T> values)
    {
        Bucket& bucket = m_data[get_index(key)];
        for (Node& node : bucket)
        {
            if (node.first == key)
            {
                for (T& value : values)
                    node.second.emplace_front(std::move(value));
                return;
            }
        }
        if (bucket.size() != m_max_bucket_size)
        {
            if (bucket.empty())
                ++not_empty_count;
            bucket.emplace_front(key, std::move(values));
        }
        else
        {
            rehash();
            emplace(key, std::move(values));
        }
    }

    /**
     * Ищет в хэш-таблице элементы с заданным ключом
     * @param[in] key ключ, по которому будет осуществляться поиск
     * @return `std::nullopt`, если элементов с заданным ключом не нашлось,
     * или `std::optional`, содержащий `std::forward_list` - список значений,
     * хранящихся в хэш-таблице по ключу `key`
     */
    [[nodiscard]] std::optional<std::forward_list<T>> equal_range(const Key& key) const
    {
        const Bucket& bucket = m_data[get_index(key)];
#ifndef NDEBUG
        assert(bucket.size() <= m_max_bucket_size);
#endif
        for (const Node& node : bucket)
            if (node.first == key)
                return node.second;
        return std::nullopt;
    }

private:
    using Node = std::pair<Key, std::forward_list<T>>;
    using Bucket = std::list<Node>;
    Hash m_hash;
    std::vector<Bucket> m_data;
    std::size_t m_max_bucket_size = 3;
    std::size_t not_empty_count = 0;

    [[nodiscard]] std::size_t get_index(const Key& key, std::size_t buckets = 0) const
    {
        if (buckets == 0)
            buckets = m_data.size();
        return m_hash(key) % buckets;
    }

    void rehash()
    {
#ifndef NDEBUG
        std::cerr << "Rehash: " << m_data.size() << ", " << m_max_bucket_size;
#endif
        std::size_t new_size, new_max_bucket_size;
        if (not_empty_count * 4 > m_data.size())
        {
            new_size = m_data.size() * 2;
            new_max_bucket_size = 3;
        }
        else
        {
            new_size = m_data.size();
            new_max_bucket_size = m_max_bucket_size + 1;
        }
#ifndef NDEBUG
        std::cerr << " --> " << new_size << ", " << new_max_bucket_size << std::endl;
#endif
        HashTable new_table;
        new_table.m_max_bucket_size = new_max_bucket_size;
        new_table.m_data.resize(new_size);

        for (Bucket& bucket : m_data)
        {
            for (Node& node : bucket)
            {
                new_table.emplace(std::move(node.first), std::move(node.second));
            }
        }
        std::swap(*this, new_table);
    }
};

} // namespace my

#endif // HASH_TABLE_H
