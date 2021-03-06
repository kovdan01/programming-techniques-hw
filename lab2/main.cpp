#include "entry.h"
#include "io_operations.h"
#include "binary_search.h"
#include "linear_search.h"
#include "../lab1/quick_sort.h"
#include <boost/program_options.hpp>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <random>
#include <type_traits>

using ArraySize = std::size_t;
using AlgoName = std::string;
using Time = std::int64_t;
using Data = std::vector<Entry>;
using Iterator = Data::iterator;
using SizeToTime = std::map<ArraySize, Time>;
using TestResult = std::map<AlgoName, SizeToTime>;

static std::mt19937 prng(std::random_device{}());

enum class Algorithm
{
    LINEAR_SEARCH,
    MY_BINARY_SEARCH,
    MY_SORT_AND_BINARY_SEARCH,
    STD_BINARY_SEARCH,
    STD_SORT_AND_BINARY_SEARCH,
    MULTIMAP,
};

static const std::vector<Algorithm> algos = {Algorithm::LINEAR_SEARCH, Algorithm::MY_BINARY_SEARCH, Algorithm::MY_SORT_AND_BINARY_SEARCH,
                                             Algorithm::STD_BINARY_SEARCH, Algorithm::STD_SORT_AND_BINARY_SEARCH, Algorithm::MULTIMAP};

std::vector<Entry::Club> pick_random_elements(Data::const_iterator begin, Data::const_iterator end, std::size_t length)
{
    std::ptrdiff_t size = std::distance(begin, end);
    std::uniform_int_distribution<std::ptrdiff_t> dist(0, size - 1);
    std::vector<Entry::Club> answer;
    for (std::size_t i = 0; i < length; ++i)
        answer.push_back(std::next(begin, dist(prng))->club());
    return answer;
}

TestResult test_all(const Data& data, const std::vector<ArraySize>& sizes)
{
    auto key_extractor = [](const Entry& entry)
    {
        return entry.club();
    };

    TestResult answer;
    const std::size_t SEARCH_COUNT = 50;

    for (ArraySize size : sizes)
    {
        size = std::min(size, data.size());
        Data::const_iterator data_size_it = std::next(data.begin(), static_cast<std::ptrdiff_t>(size));
        std::vector<Entry::Club> elements_to_search = pick_random_elements(data.begin(), data_size_it, SEARCH_COUNT);
#ifndef NDEBUG
        std::map<Algorithm, std::vector<std::size_t>> num_of_elems_found;
#endif
        for (Algorithm algo : algos)
        {
            using namespace std::chrono;
            time_point<high_resolution_clock> start;
            SizeToTime* current_algo_result;
            auto add_timing = [&start, size, &current_algo_result]()
            {
                time_point<high_resolution_clock> end = high_resolution_clock::now();
                (*current_algo_result)[size] += duration_cast<std::chrono::nanoseconds>(end - start).count();
            };

            switch (algo)
            {
            case Algorithm::LINEAR_SEARCH:
            {
                current_algo_result = &answer["Linear search"];
                for (const Entry::Club& element_to_search : elements_to_search)
                {
                    start = high_resolution_clock::now();
                    std::vector<Data::const_iterator> elements = my::find(data.begin(), data_size_it, element_to_search,
                                                                          [](const Entry& elem, const Entry::Club& key){ return elem.club() == key; });
                    add_timing();
#ifndef NDEBUG
                    num_of_elems_found[Algorithm::LINEAR_SEARCH].push_back(elements.size());
#endif
                }
                break;
            }
            case Algorithm::MY_BINARY_SEARCH:
            {
                current_algo_result = &answer["My binary search"];
                Data data_copy(data.begin(), data_size_it);
                my::quick_sort(data_copy.begin(), data_copy.end());
                for (const Entry::Club& element_to_search : elements_to_search)
                {
                    start = high_resolution_clock::now();
                    auto [range_begin, range_end] = my::equal_range(data_copy.begin(), data_copy.end(), element_to_search, key_extractor);
                    add_timing();
#ifndef NDEBUG
                    num_of_elems_found[Algorithm::MY_BINARY_SEARCH].push_back(static_cast<std::size_t>(range_end - range_begin));
#endif
                }
                break;
            }
            case Algorithm::MY_SORT_AND_BINARY_SEARCH:
            {
                current_algo_result = &answer["My sort & binary search"];
                for (const Entry::Club& element_to_search : elements_to_search)
                {
                    Data data_copy(data.begin(), data_size_it);
                    start = high_resolution_clock::now();
                    my::quick_sort(data_copy.begin(), data_copy.end());
                    auto [range_begin, range_end] = my::equal_range(data_copy.begin(), data_copy.end(), element_to_search, key_extractor);
                    add_timing();
#ifndef NDEBUG
                    num_of_elems_found[Algorithm::MY_SORT_AND_BINARY_SEARCH].push_back(static_cast<std::size_t>(range_end - range_begin));
#endif
                }
                break;
            }
            case Algorithm::STD_BINARY_SEARCH:
            {
                current_algo_result = &answer["STD binary search"];
                Data data_copy(data.begin(), data_size_it);
                std::sort(data_copy.begin(), data_copy.end());
                for (const Entry::Club& element_to_search : elements_to_search)
                {
                    start = high_resolution_clock::now();
                    auto [range_begin, range_end] = std::equal_range(data_copy.begin(), data_copy.end(), element_to_search, std::less<Entry::Club>());
                    add_timing();
                }
                break;
            }
            case Algorithm::STD_SORT_AND_BINARY_SEARCH:
            {
                current_algo_result = &answer["STD sort & binary search"];
                for (const Entry::Club& element_to_search : elements_to_search)
                {
                    Data data_copy(data.begin(), data_size_it);
                    start = high_resolution_clock::now();
                    std::sort(data_copy.begin(), data_copy.end());
                    auto [range_begin, range_end] = std::equal_range(data_copy.begin(), data_copy.end(), element_to_search, std::less<Entry::Club>());
                    add_timing();
                }
                break;
            }
            case Algorithm::MULTIMAP:
            {
                current_algo_result = &answer["Multimap"];
                std::multimap<Entry::Club, Entry> mmap;
                for (Data::const_iterator it = data.begin(); it != data_size_it; ++it)
                    mmap.emplace(it->club(), *it);
                for (const Entry::Club& element_to_search : elements_to_search)
                {
                    start = high_resolution_clock::now();
                    auto [range_begin, range_end] = mmap.equal_range(element_to_search);
                    add_timing();
#ifndef NDEBUG
                    num_of_elems_found[Algorithm::MULTIMAP].push_back(mmap.count(element_to_search));
#endif
                }
                break;
            }
            }
            (*current_algo_result)[size] /= elements_to_search.size();
        }
#ifndef NDEBUG
        std::vector<std::size_t> ethalon = num_of_elems_found[Algorithm::MULTIMAP];
        assert(ethalon == num_of_elems_found[Algorithm::LINEAR_SEARCH]);
        assert(ethalon == num_of_elems_found[Algorithm::MY_BINARY_SEARCH]);
        assert(ethalon == num_of_elems_found[Algorithm::MY_SORT_AND_BINARY_SEARCH]);
#endif
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
        ("output,O", po::value<std::string>()->required(), "csv file to write test results, the format is:\n"
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
    std::string output_filename = vm["output"].as<std::string>();
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

    // csv header
    std::ofstream output(output_filename);
    output << "name";
    for (ArraySize size : sizes)
        output << ';' << size;
    output << '\n';

    TestResult results = test_all(data, sizes);
    for (auto& [name, timings] : results)
    {
        std::cerr << std::endl << "Algorithm: " << name << std::endl;
        for (auto [size, time] : timings)
            std::cerr << size << ": " << time << std::endl;
        print_timings_csv_line(output, name, timings);
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
