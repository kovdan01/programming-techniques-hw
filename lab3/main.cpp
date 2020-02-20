#include "entry.h"
#include "io_operations.h"
#include "dummy.h"
#include "elf.h"
#include "rot13.h"
#include "hash_table.h"
#include <boost/program_options.hpp>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <functional>
#include <random>
#include <type_traits>

using ArraySize = std::size_t;
using HashName = std::string;
using Time = std::int64_t;
using Data = std::vector<Entry>;
using Iterator = Data::iterator;
using SizeToTime = std::map<ArraySize, Time>;
using SizeToPercentage = std::map<ArraySize, double>;
using TestTimeResult = std::map<HashName, SizeToTime>;
using TestCollisionResult = std::map<HashName, SizeToPercentage>;

static std::mt19937 prng(std::random_device{}());

enum class HashAlgorithm
{
    STDHASH,
    DUMMY,
    ROT13,
    ROT19,
    ELF,
};

static const std::map<HashAlgorithm, HashName> hash_names =
{
    { HashAlgorithm::STDHASH, "std::hash" },
    { HashAlgorithm::DUMMY,   "dummy" },
    { HashAlgorithm::ROT13,   "rot13" },
    { HashAlgorithm::ROT19,   "rot19" },
    { HashAlgorithm::ELF,     "elf" },
};

std::vector<Entry::Trainer> pick_random_elements(Data::const_iterator begin, Data::const_iterator end, std::size_t length)
{
    std::ptrdiff_t size = std::distance(begin, end);
    std::uniform_int_distribution<std::ptrdiff_t> dist(0, size - 1);
    std::vector<Entry::Trainer> answer;
    for (std::size_t i = 0; i < length; ++i)
        answer.push_back(std::next(begin, dist(prng))->trainer());
    return answer;
}

template <typename Hash>
SizeToTime test_hash_timings(const Data& data, const std::map<std::size_t, std::vector<Entry::Trainer>>& size_to_elements)
{
    SizeToTime answer;
    using namespace std::chrono;
    for (auto& [_size, elements] : size_to_elements)
    {
        std::size_t size = std::min(_size, data.size());
        Data::const_iterator data_size_it = std::next(data.begin(), static_cast<std::ptrdiff_t>(size));
        my::HashTable<Entry::Trainer, Entry, Hash> mmap;
        for (Data::const_iterator it = data.begin(); it != data_size_it; ++it)
            mmap.emplace(it->trainer(), *it);
#ifndef NDEBUG
        std::unordered_multimap<Entry::Trainer, Entry, Hash> std_mmap;
        for (Data::const_iterator it = data.begin(); it != data_size_it; ++it)
            std_mmap.emplace(it->trainer(), *it);
#endif
        time_point<high_resolution_clock> start = high_resolution_clock::now();
        for (const Entry::Trainer& element_to_search : elements)
        {
            const std::forward_list<Entry>& found = mmap.equal_range(element_to_search);
#ifndef NDEBUG
            auto [range_begin, range_end] = std_mmap.equal_range(element_to_search);
            assert(std::distance(found.begin(), found.end()) == std::distance(range_begin, range_end));
#endif
        }
        time_point<high_resolution_clock> end = high_resolution_clock::now();
        answer[size] = static_cast<Time>(
                           duration_cast<std::chrono::nanoseconds>(end - start).count() /
                           static_cast<double>(elements.size())
                       );
    }

    return answer;
}

TestTimeResult test_all_timings(const Data& data, const std::vector<ArraySize>& sizes)
{
    const std::size_t SEARCH_COUNT = 1000;
    TestTimeResult answer;

    std::map<std::size_t, std::vector<Entry::Trainer>> elements_to_search;
    for (ArraySize size : sizes)
    {
        size = std::min(size, data.size());
        if (elements_to_search.contains(size))
            continue;
        Data::const_iterator data_size_it = std::next(data.begin(), static_cast<std::ptrdiff_t>(size));
        elements_to_search[size] = pick_random_elements(data.begin(), data_size_it, SEARCH_COUNT);
    }

    for (auto& [algo, name] : hash_names)
    {
        std::cerr << "Testing timings for " << name << "..." << std::endl;
        switch (algo)
        {
        case HashAlgorithm::STDHASH:
            answer.emplace(name, test_hash_timings<std::hash<Entry::Trainer>>(data, elements_to_search));
            break;
        case HashAlgorithm::DUMMY:
            answer.emplace(name, test_hash_timings<my::DummyHash>(data, elements_to_search));
            break;
        case HashAlgorithm::ROT13:
            answer.emplace(name, test_hash_timings<my::Rot13Hash>(data, elements_to_search));
            break;
        case HashAlgorithm::ROT19:
            answer.emplace(name, test_hash_timings<my::Rot19Hash>(data, elements_to_search));
            break;
        case HashAlgorithm::ELF:
            answer.emplace(name, test_hash_timings<my::ElfHash>(data, elements_to_search));
            break;
        }
        std::cerr << "Done!" << std::endl;
    }

    return answer;
}

template <typename Hash>
SizeToPercentage test_hash_collisions(const std::map<std::size_t, std::vector<std::string>>& size_to_elements)
{
    SizeToPercentage answer;
    Hash hasher;
    for (auto& [size, elements] : size_to_elements)
    {
        std::map<std::size_t, std::size_t> collision_count;
        for (const std::string& element : elements)
            ++collision_count[hasher(element)];
        std::size_t collisions = 0;
        for (auto [hash, number] : collision_count)
            if (number > 1)
                collisions += number;
        answer[size] = static_cast<double>(collisions) * 100 / size;
    }

    return answer;
}

TestCollisionResult test_all_collisions(const Data& data, const std::vector<ArraySize>& sizes)
{
    std::vector<std::string> strings;
    strings.reserve(data.size());
    for (const Entry& entry : data)
        strings.emplace_back(entry.country() + entry.city() + entry.club() + entry.trainer());
    std::sort(strings.begin(), strings.end());
    std::vector<std::string>::iterator end_it = std::unique(strings.begin(), strings.end());
    strings.resize(static_cast<std::size_t>(std::distance(strings.begin(), end_it)));
    std::shuffle(strings.begin(), strings.end(), prng);

    TestCollisionResult answer;
    std::map<std::size_t, std::vector<std::string>> elements;
    for (ArraySize size : sizes)
    {
        size = std::min(size, strings.size());
        if (elements.contains(size))
            continue;
        std::vector<std::string>::const_iterator strings_size_it = std::next(strings.begin(), static_cast<std::ptrdiff_t>(size));
        elements[size] = std::vector<std::string>(strings.cbegin(), strings_size_it);
    }

    for (auto& [algo, name] : hash_names)
    {
        std::cerr << "Testing collisions for " << name << "..." << std::endl;
        switch (algo)
        {
        case HashAlgorithm::STDHASH:
            answer.emplace(name, test_hash_collisions<std::hash<std::string>>(elements));
            break;
        case HashAlgorithm::DUMMY:
            answer.emplace(name, test_hash_collisions<my::DummyHash>(elements));
            break;
        case HashAlgorithm::ROT13:
            answer.emplace(name, test_hash_collisions<my::Rot13Hash>(elements));
            break;
        case HashAlgorithm::ROT19:
            answer.emplace(name, test_hash_collisions<my::Rot19Hash>(elements));
            break;
        case HashAlgorithm::ELF:
            answer.emplace(name, test_hash_collisions<my::ElfHash>(elements));
            break;
        }
        std::cerr << "Done!" << std::endl;
    }

    return answer;
}

int main(int argc, char* argv[]) try
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,H", "Print this message")
        ("sizes,S", po::value<std::string>()->required(), "Text file with data sizes to be testes in the following format:\n"
                                                          "size_0 size_1 size_2 ... size_n")

        ("input,I", po::value<std::string>()->required(), "File (csv or sqlite) with football clubs data. Format:\n"
                                                          "* if csv: country;club;city;trainer;year;score\n"
                                                          "* if sqlite: table 'entries' with columns 'country', 'club', 'city', 'trainer', 'year', 'score'")
        ("format,F", po::value<std::string>(), "Input file format (csv or sqlite)")
        ("output_time,T", po::value<std::string>()->required(), "csv file to write test timing results, the format is:\n"
                                                                "algo_name;result_for_size_0;...;result_for_size_n")
        ("output_collision,C", po::value<std::string>()->required(), "csv file to write test collision results, the format is:\n"
                                                                     "algo_name;result_for_size_0;...;result_for_size_n")
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

    std::string input_filename = vm["input"].as<std::string>();
    std::string sizes_filename = vm["sizes"].as<std::string>();
    std::string output_time_filename = vm["output_time"].as<std::string>();
    std::string output_collision_filename = vm["output_collision"].as<std::string>();
    std::string format;
    if (vm.contains("format"))
    {
        format = vm["format"].as<std::string>();
    }
    else
    {
        if (input_filename.ends_with(".csv"))
        {
            format = "csv";
        }
        else if (input_filename.ends_with(".sqlite"))
        {
            format = "sqlite";
        }
        else
        {
            std::cerr << "Invalid format. Please either specify format manually with "
                         "--format or use --input with extension .csv or .sqlite.\n"
                         "Please use --help to see detailed help message";
        }
    }

    if (format != "csv" && format != "sqlite")
    {
        std::cerr << "Invalid format. Please use --help see help message\n";
        return 1;
    }

    std::cerr << "Reading data..." << std::endl;
    Data data;
    if (format == "csv")
        data = read_data_from_csv(input_filename);
    else if (format == "sqlite")
        data = read_data_from_sqlite(input_filename);
    std::vector<ArraySize> sizes = read_sizes(sizes_filename);
    shrink_sizes(sizes, data.size());
    std::cerr << "Done!" << std::endl;

    // timings
    {
        // csv header
        std::ofstream output(output_time_filename);
        output << "name";
        for (ArraySize size : sizes)
            output << ';' << size;
        output << '\n';

        TestTimeResult results = test_all_timings(data, sizes);
        std::cerr << "Timings:\n";
        for (auto& [name, timings] : results)
        {
            std::cerr << std::endl << "Algorithm: " << name << std::endl;
            for (auto [size, time] : timings)
                std::cerr << size << ": " << time << std::endl;
            print_timings_csv_line(output, name, timings);
        }
    }

    // collisions
    {
        // csv header
        std::ofstream output(output_collision_filename);
        output << "name";
        for (ArraySize size : sizes)
            output << ';' << size;
        output << '\n';

        TestCollisionResult results = test_all_collisions(data, sizes);
        std::cerr << "Collisions:" << std::endl;
        for (auto& [name, percentage] : results)
        {
            std::cerr << std::endl << "Algorithm: " << name << std::endl;
            for (auto [size, time] : percentage)
                std::cerr << size << ": " << time << std::endl;
            print_collisions_csv_line(output, name, percentage);
        }
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
