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
        : m_hasher({})
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
        std::size_t hash = m_hasher(key);
        Bucket& bucket = m_data[hash % m_data.size()];
        emplace(key, std::move(values), bucket, hash);
    }

    /**
     * Ищет в хэш-таблице элементы с заданным ключом
     * @param[in] key ключ, по которому будет осуществляться поиск
     * @return `std::forward_list`, содержащий значения,
     * хранящиеся в хэш-таблице по ключу `key`
     */
    [[nodiscard]] const std::forward_list<T>& equal_range(const Key& key) const
    {
        const Bucket& bucket = m_data[get_index(key)];
#ifndef NDEBUG
        assert(bucket.size() <= m_max_bucket_size);
#endif
        for (const Node& node : bucket)
            if (node.hash_and_key().key() == key)
                return node.values();
        return m_empty_list;
    }

private:
    class HashAndKey
    {
    public:
        HashAndKey(std::size_t hash, Key key)
            : m_hash(hash), m_key(std::move(key))
        {}
        HashAndKey() = delete;
        HashAndKey(const HashAndKey&) = default;
        HashAndKey& operator=(const HashAndKey&) = default;
        HashAndKey(HashAndKey&&) noexcept = default;
        HashAndKey& operator=(HashAndKey&&) noexcept = default;

        [[nodiscard]] std::size_t hash() const { return m_hash; }

        [[nodiscard]] const Key& key() const { return m_key; }
        [[nodiscard]] Key& key() { return m_key; }

    private:
        std::size_t m_hash;
        Key m_key;
    };

    class Node
    {
    public:
        Node(HashAndKey hash_and_key, std::forward_list<T> values)
            : m_hash_and_key(std::move(hash_and_key)), m_values(std::move(values))
        {}
        Node() = delete;
        Node(const Node&) = default;
        Node& operator=(const Node&) = default;
        Node(Node&&) noexcept = default;
        Node& operator=(Node&&) noexcept = default;

        [[nodiscard]] const HashAndKey& hash_and_key() const { return m_hash_and_key; }
        [[nodiscard]] HashAndKey& hash_and_key() { return m_hash_and_key; }

        [[nodiscard]] const std::forward_list<T>& values() const { return m_values; }
        [[nodiscard]] std::forward_list<T>& values() { return m_values; }

    private:
        HashAndKey m_hash_and_key;
        std::forward_list<T> m_values;
    };

    using Bucket = std::list<Node>;

    Hash m_hasher;
    std::vector<Bucket> m_data;
    std::size_t m_max_bucket_size = 3;
    std::size_t not_empty_count = 0;

    static inline const std::forward_list<T> m_empty_list{};

    [[nodiscard]] std::size_t get_index(const Key& key) const
    {
        return m_hasher(key) % m_data.size();
    }

    void emplace(const Key& key, std::forward_list<T> values, std::size_t hash)
    {
        Bucket& bucket = m_data[hash % m_data.size()];
        emplace(key, std::move(values), bucket, hash);
    }

    void emplace(const Key& key, std::forward_list<T> values, Bucket& bucket, std::size_t hash)
    {
        for (Node& node : bucket)
        {
            if (node.hash_and_key().key() == key)
            {
                for (T& value : values)
                    node.values().emplace_front(std::move(value));
                return;
            }
        }
        if (bucket.size() != m_max_bucket_size)
        {
            if (bucket.empty())
                ++not_empty_count;
            bucket.emplace_front(HashAndKey(hash, key), std::move(values));
        }
        else
        {
            rehash();
            emplace(key, std::move(values), hash);
        }
    }

    void rehash()
    {
        std::size_t new_size, new_max_bucket_size;
        if (not_empty_count * 7 > m_data.size())
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
        std::cerr << "Rehash: " << m_data.size() << ", " << m_max_bucket_size
                  << " --> " << new_size << ", " << new_max_bucket_size << std::endl;
#endif
        HashTable new_table;
        new_table.m_max_bucket_size = new_max_bucket_size;
        new_table.m_data.resize(new_size);

        for (Bucket& bucket : m_data)
            for (Node& node : bucket)
                new_table.emplace(std::move(node.hash_and_key().key()), std::move(node.values()), node.hash_and_key().hash());

        std::swap(*this, new_table);
    }
};

} // namespace my

#endif // HASH_TABLE_H
