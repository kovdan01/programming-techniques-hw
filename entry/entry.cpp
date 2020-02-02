#include "entry.h"
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <tuple>

bool operator==(const Entry& lhs, const Entry& rhs)
{
    return std::tie(lhs.m_club, lhs.m_year, lhs.m_country, lhs.m_score) ==
           std::tie(rhs.m_club, rhs.m_year, rhs.m_country, rhs.m_score);
}

bool operator!=(const Entry& lhs, const Entry& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const Entry& lhs, const Entry& rhs)
{
    double lhs_reversed_score = 1. / lhs.m_score;
    double rhs_reversed_score = 1. / rhs.m_score;
    return std::tie(lhs.m_club, lhs.m_year, lhs.m_country, lhs_reversed_score) <
           std::tie(rhs.m_club, rhs.m_year, rhs.m_country, rhs_reversed_score);
}

bool operator>(const Entry& lhs, const Entry& rhs)
{
    return (rhs < lhs);
}

bool operator<=(const Entry& lhs, const Entry& rhs)
{
    return !(lhs > rhs);
}
bool operator>=(const Entry& lhs, const Entry& rhs)
{
    return !(lhs < rhs);
}

void Entry::to_csv(std::ostream& stream, char sep) const
{
    stream << m_country << sep
           << m_city << sep
           << m_club << sep
           << m_trainer << sep
           << m_year << sep
           << m_score << '\n';
}

void Entry::to_sqlite(SQLite::Database &db, const std::string& table) const
{
    db.exec("INSERT INTO " + table + " "
            "(country, city, club, trainer, year, score)"
            "VALUES ("
            + "\"" + m_country        + "\", "
            + "\"" + m_city           + "\", "
            + "\"" + m_club           + "\", "
            + "\"" + m_trainer        + "\", "
            + std::to_string(m_year)  + ", "
            + std::to_string(m_score) + ")");
}

Entry from_csv(const std::string& csv_line, char sep)
{
    Entry::Country country;
    Entry::City city;
    Entry::Club club;
    Entry::Trainer trainer;
    Entry::Year year;
    Entry::Score score;
    std::string remaining;
    std::istringstream input(csv_line);
    getline(input, country, sep);
    getline(input, city, sep);
    getline(input, club, sep);
    getline(input, trainer, sep);
    input >> year;
    if (input.peek() != sep)
        throw std::runtime_error("Invalid separator or wrong format of year");
    input.ignore(1);
    input >> score;
    getline(input, remaining);
    if (!remaining.empty())
        throw std::runtime_error("Invalid csv");
    return Entry(country, city, club, trainer, year, score);
}

Entry from_sqlite(SQLite::Statement& query)
{
    Entry::Country country = query.getColumn("country");
    Entry::City city = query.getColumn("city");
    Entry::Club club = query.getColumn("club");
    Entry::Trainer trainer = query.getColumn("trainer");
    Entry::Year year = query.getColumn("year");
    Entry::Score score = query.getColumn("score");
    return Entry(country, city, club, trainer, year, score);
}

std::ostream& operator<<(std::ostream& stream, const Entry& entry)
{
    stream << "country: \"" << entry.m_country
           << "\"\ncity: \"" << entry.m_city
           << "\"\nclub: \"" << entry.m_club
           << "\"\ntrainer: \"" << entry.m_trainer
           << "\"\nyear: " << entry.m_year
           << "\nscore: " << entry.m_score << '\n';
    return stream;
}
