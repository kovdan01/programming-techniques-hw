#include "entry.h"
#include "geography.h"
#include "names.h"
#include "teams.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include <stdexcept>

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

Entry generate_entry(std::mt19937& prng)
{
    auto [country, city] = generate_location(prng);
    std::string club = generate_team(prng);
    std::string trainer = generate_trainer(prng);
    int year = generate_year(prng);
    int score = generate_score(prng);
    return Entry(country, city, club, trainer, year, score);
}

int main(int argc, char* argv[]) try
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,H", "Print this message")
        ("size,S", po::value<std::size_t>()->required(), "Number of entries to generate (required)")
        ("output,O", po::value<std::string>()->required(), "Filename to store entries (required)")
        ("format,F", po::value<std::string>(), "File format (csv or sqlite)")
        ;

    po::variables_map vm;
    try
    {
        po::store(parse_command_line(argc, argv, desc), vm);
        if (vm.contains("help"))
        {
            std::cout << desc << "\n";
            return 0;
        }
        po::notify(vm);
    }
    catch (const po::error& error)
    {
        std::cerr << "Error while parsing command-line arguments: "
                  << error.what() << "\nPlease use --help to see help message\n";
        return 1;
    }

    std::string filename = vm["output"].as<std::string>();
    std::size_t size = vm["size"].as<std::size_t>();
    std::string format;
    if (vm.contains("format"))
    {
        format = vm["format"].as<std::string>();
    }
    else
    {
        if (filename.ends_with(".csv"))
        {
            format = "csv";
        }
        else if (filename.ends_with(".sqlite"))
        {
            format = "sqlite";
        }
        else
        {
            std::cerr << "Invalid format. Please either specify format manually with "
                         "--format or use --output with extension .csv or .sqlite.\n"
                         "Please use --help to see detailed help message";
        }
    }

    if (format != "csv" && format != "sqlite")
    {
        std::cerr << "Invalid format. Please use --help see help message\n";
        return 1;
    }

    std::mt19937 prng(std::random_device{}());

    if (format == "csv")
    {
        std::ofstream csv(filename);
        csv << "country;city;club;trainer;year;score\n";
        for (std::size_t i = 0; i < size; ++i)
            generate_entry(prng).to_csv(csv);
    }
    else if (format == "sqlite")
    {
        SQLite::Database db(filename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        db.exec("DROP TABLE IF EXISTS " + Entry::table_name);
        db.exec("CREATE TABLE " + Entry::table_name + " ("
                "country  TEXT, "
                "city     TEXT, "
                "club     TEXT, "
                "trainer  TEXT, "
                "year  INTEGER, "
                "score INTEGER)");

        SQLite::Transaction transaction(db);
        for (std::size_t i = 0; i < size; ++i)
            generate_entry(prng).to_sqlite(db, Entry::table_name);
        transaction.commit();
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
