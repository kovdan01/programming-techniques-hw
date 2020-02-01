#include "io_operations.h"
#include <algorithm>
#include <fstream>
#include <ostream>
#include <map>
#include <string>
#include <vector>

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

Data read_data(const std::string& csv_filename, char sep)
{
    std::ifstream input(csv_filename);
    std::string csv_line;
    std::getline(input, csv_line);
    Data answer;
    while (std::getline(input, csv_line))
        answer.emplace_back(from_csv(csv_line, sep));
    return answer;
}

std::ostream& print_timings_csv_line(std::ostream& output, const AlgoName& name,
                                     const SizeToTime& timings, char sep)
{
    output << name;
    for (auto [_, time] : timings)
        output << sep << time;
    output << '\n';
    return output;
}
