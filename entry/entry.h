/**
 * @file
 * @brief Заголовочный файл, содержащий описание класса Entry
 * @date Январь 2020
*/
#ifndef ENTRY_H
#define ENTRY_H

#include "SQLiteCpp/SQLiteCpp.h"
#include <ostream>
#include <string>
#include <utility>

/**
 * @class Entry
 * @brief Класс, содержащий описание футбольной команды
 */
class Entry
{
public:
    using Country = std::string;
    using City = std::string;
    using Club = std::string;
    using Trainer = std::string;
    using Year = int;
    using Score = int;

    Entry() = delete;

    Entry(Country country, City city, Club club,
          Trainer trainer, Year year, Score score)
        : m_country(std::move(country))
        , m_city(std::move(city))
        , m_club(std::move(club))
        , m_trainer(std::move(trainer))
        , m_year(year), m_score(score)
    {}

    Entry(const Entry&) = default;
    Entry& operator=(const Entry&) = default;
    Entry(Entry&&) noexcept = default;
    Entry& operator=(Entry&&) noexcept = default;

    [[nodiscard]] Country country() const { return m_country; }
    [[nodiscard]] City city() const { return m_city; }
    [[nodiscard]] Club club() const { return m_club; }
    [[nodiscard]] Trainer trainer() const { return m_trainer; }
    [[nodiscard]] Year year() const { return m_year; }
    [[nodiscard]] Score score() const { return m_score; }

    /**
     * Выводит данные о классе в заданный поток вывода в формате csv:
     * country;city;club;trainer;year;score
     * @param[out] stream поток вывода, в который производится вывод
     * @param[in] sep разделитель, использующийся в формате csv
     */
    void to_csv(std::ostream& stream, char sep = ';') const;

    inline static const std::string table_name = "entries";
    /**
     * Выводит данные о классе в заданный поток вывода в базу данных SQLite
     * @param[out] db объект базы данных, в которую нужно произвести запись
     * @param[in] table имя таблицы в базе `db`, в которую производится запись;
     * должна иметь поля `country (TEXT), city (TEXT), club (TEXT), `
     * `trainer (TEXT), year (INTEGER), score (INTEGER)`
     */
    void to_sqlite(SQLite::Database& db, const std::string& table = table_name) const;

    friend bool operator==(const Entry& lhs, const Entry& rhs);
    friend bool operator!=(const Entry& lhs, const Entry& rhs);
    friend bool operator<(const Entry& lhs, const Entry& rhs);
    friend bool operator>(const Entry& lhs, const Entry& rhs);
    friend bool operator<=(const Entry& lhs, const Entry& rhs);
    friend bool operator>=(const Entry& lhs, const Entry& rhs);
    friend std::ostream& operator<<(std::ostream& stream, const Entry& entry);

    operator Club() { return m_club; }
private:
    Country m_country;
    City m_city;
    Club m_club;
    Trainer m_trainer;
    Year m_year;
    Score m_score;
};

/**
 * Создает объект класса `Entry` из строки в формате csv
 * @param[in] csv_line строка в формате csv со следующими столбцами:
 * country;city;club;trainer;year;score
 * @param[in] sep разделитель, использующийся в формате csv
 * @return созданный по строке в формате csv объект класса `Entry`
 */
Entry from_csv(const std::string& csv_line, char sep = ';');

/**
 * Создает объект класса `Entry` по данным из БД SQLite
 * @param[in] query сформированный SQL-запрос; будут использованы поля:
 * country (TEXT), city (TEXT), club (TEXT), trainer (TEXT), `
 * `year (INTEGER), score (INTEGER)`
 * @return созданный по данным из БД объект класса `Entry`
 */
Entry from_sqlite(SQLite::Statement& query);

#endif // ENTRY_H
