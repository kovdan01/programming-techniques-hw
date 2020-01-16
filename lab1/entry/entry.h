#ifndef ENTRY_H
#define ENTRY_H

#include <ostream>
#include <string>
#include <utility>

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
    Entry(Entry&&) = default;
    Entry& operator=(Entry&&) = default;

    [[nodiscard]] Country country() const { return m_country; }
    [[nodiscard]] City city() const { return m_city; }
    [[nodiscard]] Club club() const { return m_club; }
    [[nodiscard]] Trainer trainer() const { return m_trainer; }
    [[nodiscard]] Year year() const { return m_year; }
    [[nodiscard]] Score score() const { return m_score; }

    void to_csv(std::ostream& stream, char sep = ';') const;

    friend bool operator==(const Entry& lhs, const Entry& rhs);
    friend bool operator!=(const Entry& lhs, const Entry& rhs);
    friend bool operator<(const Entry& lhs, const Entry& rhs);
    friend bool operator>(const Entry& lhs, const Entry& rhs);
    friend bool operator<=(const Entry& lhs, const Entry& rhs);
    friend bool operator>=(const Entry& lhs, const Entry& rhs);
    friend std::ostream& operator<<(std::ostream& stream, const Entry& entry);
private:
    Country m_country;
    City m_city;
    Club m_club;
    Trainer m_trainer;
    Year m_year;
    Score m_score;
};

Entry from_csv(const std::string& csv_line, char sep = ';');

#endif // ENTRY_H
