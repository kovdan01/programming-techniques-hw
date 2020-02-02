#include "entry.h"
#include "io_operations.h"
#include "heap_sort.h"
#include "quick_sort.h"
#include "shaker_sort.h"
#include <boost/program_options.hpp>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using ArraySize = std::size_t;
using SortName = std::string;
using Time = std::int64_t;
using Data = std::vector<Entry>;
using Iterator = Data::iterator;
using SizeToTime = std::map<ArraySize, Time>;
using TestResult = std::map<SortName, SizeToTime>;

SizeToTime test_sort(const std::function<void(Iterator, Iterator)>& sort_function,
                     const Data& data, const std::vector<ArraySize>& sizes)
{
    SizeToTime answer;
    using namespace std::chrono;
    for (ArraySize size : sizes)
    {
        size = std::min(size, data.size());
        if (answer.contains(size))
            continue;
        Data data_copy(data.begin(), std::next(data.begin(), static_cast<std::ptrdiff_t>(size)));
        time_point<high_resolution_clock> start = high_resolution_clock::now();
        sort_function(data_copy.begin(), data_copy.end());
        time_point<high_resolution_clock> end = high_resolution_clock::now();
        answer[size] = duration_cast<std::chrono::nanoseconds>(end - start).count();
#ifndef DNDEBUG
        if (!std::is_sorted(data_copy.begin(), data_copy.end()))
            throw std::runtime_error("Wrong sort algorithm");
#endif
    }
    return answer;
}

TestResult test_all(const Data& data, const std::vector<ArraySize>& sizes)
{
    std::map<SortName, std::function<void(Iterator, Iterator)>> name_to_function =
    {
        { "Quick Sort", my::quick_sort<Iterator> },
        { "Heap Sort", my::heap_sort<Iterator> },
        { "Shaker Sort", my::shaker_sort<Iterator> }
    };
    TestResult answer;
    for (auto& [name, function] : name_to_function)
    {
        std::cerr << "Testing " << name << "..." << std::endl;
        answer.emplace(name, test_sort(function, data, sizes));
        std::cerr << "Done!" << std::endl;
    }
    return answer;
}

int main(int argc, char* argv[])
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
                                                           "sort_name;result_for_size_0;...;result_for_size_n")
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
