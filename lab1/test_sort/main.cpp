#include "../entry/entry.h"
#include "heap_sort.h"
#include "quick_sort.h"
#include "shaker_sort.h"
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
        if (answer.count(size) != 0)
            continue;
        Data data_copy(data.begin(), std::next(data.begin(), static_cast<std::ptrdiff_t>(size)));
        time_point<high_resolution_clock> start = high_resolution_clock::now();
        sort_function(data_copy.begin(), data_copy.end());
        time_point<high_resolution_clock> end = high_resolution_clock::now();
        answer[size] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
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

std::vector<ArraySize> read_sizes(const std::string& sizes_filename)
{
    std::ifstream sizes(sizes_filename);
    std::vector<ArraySize> answer;
    ArraySize current_size;
    while (sizes >> current_size)
        answer.emplace_back(current_size);
    return answer;
}

void shrink_sizes(std::vector<ArraySize>& sizes, ArraySize max_size)
{
    for (ArraySize& size : sizes)
        size = std::min(size, max_size);
    std::sort(sizes.begin(), sizes.end());
    auto last = std::unique(sizes.begin(), sizes.end());
    sizes.resize(static_cast<std::size_t>(last - sizes.begin()));
}

Data read_data(const std::string& csv_filename, char sep = ';')
{
    std::ifstream input(csv_filename);
    std::string csv_line;
    std::getline(input, csv_line);
    Data answer;
    while (std::getline(input, csv_line))
        answer.emplace_back(from_csv(csv_line, sep));
    return answer;
}

std::ostream& print_csv_line(std::ostream& output, const std::string& name,
                             const SizeToTime& timings, char sep = ';')
{
    output << name;
    for (auto [_, time] : timings)
        output << sep << time;
    output << '\n';
    return output;
}

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    if (argc != 4)
    {
        std::cerr << "Invalid arguments.\n"
                     "Usage: ./lab1 <input_name.csv> <sizes> <output_name.csv>\n"
                     "* <input_name.csv> - csv file with football clubs data in the following format:\n"
                     "  country;club;city;trainer;year;score\n"
                     "* <sizes> - text file with data sizes to be testes in the following format:\n"
                     "  size_0 size_1 size_2 ... size_n\n"
                     "* <output_name.csv> - csv file to write test results, the format is:\n"
                     "  sort_name;result_for_size_0;...;result_for_size_n\n";
        return 1;
    }
    std::string input_filename(argv[1]);
    std::string sizes_filename(argv[2]);
    std::string output_filename(argv[3]);

    std::cerr << "Reading data..." << std::endl;
    Data data = read_data(input_filename);
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
        print_csv_line(output, name, timings);
    }

    return 0;
}
