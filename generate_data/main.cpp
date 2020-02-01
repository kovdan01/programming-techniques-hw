#include "entry.h"
#include "geography.h"
#include "names.h"
#include "teams.h"
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

std::pair<Country, City> generate_location(std::mt19937& prng)
{
    std::uniform_int_distribution<std::size_t> dist(0, cities.size() - 1);
    return cities[dist(prng)];
}

std::string generate_team(std::mt19937& prng)
{
    std::uniform_int_distribution<std::size_t> dist(0, teams.size() - 1);
    return teams[dist(prng)];
}

std::string generate_trainer(std::mt19937& prng)
{
    std::uniform_int_distribution<std::size_t> dist_first(0, first_names.size() - 1);
    std::uniform_int_distribution<std::size_t> dist_last(0, last_names.size() - 1);
    return first_names[dist_first(prng)] + " " + last_names[dist_last(prng)];
}

int generate_year(std::mt19937& prng)
{
    return std::uniform_int_distribution<int>(1990, 2020)(prng); // NOLINT(cppcoreguidelines-avoid-magic-numbers)
}

int generate_score(std::mt19937& prng)
{
    return std::uniform_int_distribution<int>(0, 100)(prng); //NOLINT(cppcoreguidelines-avoid-magic-numbers)
}

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    if (argc != 2)
    {
        std::cerr << "Invalid arguments. Usage:\n"
                     "./generate_data <data_size>\n"
                     "<data_size> - number of lines in csv\n";
        return 1;
    }

    int size = std::stoi(std::string(argv[1]));
    std::mt19937 prng(std::random_device{}());
    std::cout << "country;city;club;trainer;year;score\n";

    for (int i = 0; i < size; ++i)
    {
        auto [country, city] = generate_location(prng);
        std::string club = generate_team(prng);
        std::string trainer = generate_trainer(prng);
        int year = generate_year(prng);
        int score = generate_score(prng);
        Entry(country, city, club, trainer, year, score).to_csv(std::cout);
    }

    return 0;
}