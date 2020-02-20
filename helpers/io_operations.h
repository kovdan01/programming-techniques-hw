#include "entry.h"
#include <ostream>
#include <map>
#include <string>
#include <vector>

using ArraySize = std::size_t;
using AlgoName = std::string;
using Time = std::int64_t;
using Data = std::vector<Entry>;
using SizeToTime = std::map<ArraySize, Time>;
using SizeToPercentage = std::map<ArraySize, double>;

std::vector<ArraySize> read_sizes(const std::string& sizes_filename);

void shrink_sizes(std::vector<ArraySize>& sizes, ArraySize max_size);

Data read_data_from_csv(const std::string& csv_filename, char sep = ';');
Data read_data_from_sqlite(const std::string& sqlite_filename);

std::ostream& print_timings_csv_line(std::ostream& output, const AlgoName& name,
                                     const SizeToTime& timings, char sep = ';');

std::ostream& print_collisions_csv_line(std::ostream& output, const AlgoName& name,
                                        const SizeToPercentage& percentages, char sep = ';');
