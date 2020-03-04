#include "io_operations.h"
#include "chi_squared.h"
#include "stat_utils.h"
#include "dummy.h"
#include "custom.h"
#include <boost/program_options.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include <limits>

using ArraySize = std::size_t;
using PRNGName = std::string;
using Time = std::int64_t;
using SizeToTime = std::map<ArraySize, Time>;
using TimingResults = std::map<PRNGName, SizeToTime>;
using IntegerType = std::uint16_t;
using IntegerSample = std::vector<IntegerType>;

template <typename Distribution, typename Number, typename Generator>
std::vector<Number> generate_sample(std::size_t size, Distribution& distribution, Generator& generator)
{
    std::vector<Number> sample(size);
    for (Number& value : sample)
        value = distribution(generator);
    return sample;
}

template <typename Integer, typename Generator>
std::vector<Integer> generate_integer_sample(std::size_t size, Integer min, Integer max, Generator& generator)
{
    std::uniform_int_distribution<Integer> dist(min, max);
    return generate_sample<std::uniform_int_distribution<Integer>, Integer, Generator>(size, dist, generator);
}

void test_samples(const std::vector<IntegerSample>& samples, IntegerType min, IntegerType max, double significance_level)
{
    std::size_t accepted_count = 0;
    for (auto sample : samples | boost::adaptors::indexed(0))
    {
        std::cout << std::setw(4) << sample.index() << ' ';
        std::cout << "Chi squared: ";
        if (my::check_chi_squared_criteria(sample.value(), min, max, significance_level, my::DistributionType::UNIFORM_INT))
        {
            std::cout << "accept.  ";
            ++accepted_count;
        }
        else
        {
            std::cout << "decline. ";
        }
        std::cout << std::internal << std::setprecision(2) << "Sample: "
                  << "mean = "        << std::fixed << std::setw(8) << my::stat::mean(sample.value()) << ", "
                  << "variance = "    << std::fixed << std::setw(8) << my::stat::variance(sample.value()) << ", "
                  << "variability = " << std::fixed << std::setw(4) << my::stat::variability(sample.value()) << '\n';
    }
    std::cout << "TOTAL: accepted " << accepted_count << " out of " << samples.size() << " samples (" << 100.0 * accepted_count / samples.size() << "%)\n";
}

template <typename Generator>
void test_generator(const std::string& name, std::size_t sample_count, std::size_t sample_size,
                    IntegerType min, IntegerType max, double significance_level)
{
    std::cout << "Testing " << name << '\n';

    Generator prng(std::random_device{}());
    std::vector<IntegerSample> samples(sample_count);
    for (IntegerSample& sample : samples)
        sample = generate_integer_sample<IntegerType, Generator>(sample_size, min, max, prng);

    test_samples(samples, min, max, significance_level);
}

void test_all_generators(std::size_t sample_count, std::size_t sample_size,
                         IntegerType min, IntegerType max, double significance_level)
{
    test_generator<std::mt19937>     ("mt19937",      sample_count, sample_size, min, max, significance_level);
    test_generator<std::minstd_rand0>("minstd_rand0", sample_count, sample_size, min, max, significance_level);
    test_generator<my::DummyPRNG>    ("dummy",        sample_count, sample_size, min, max, significance_level);
    test_generator<my::CustomPRNG>   ("custom",       sample_count, sample_size, min, max, significance_level);
}

template <typename Generator>
SizeToTime test_prng_timing(const std::vector<ArraySize>& sample_sizes, IntegerType min, IntegerType max)
{
    Generator prng(std::random_device{}());
    SizeToTime answer;
    for (ArraySize sample_size : sample_sizes)
    {
        if (answer.contains(sample_size))
            continue;
        using namespace std::chrono;
        time_point<high_resolution_clock> begin = high_resolution_clock::now();
        IntegerSample sample = generate_integer_sample<IntegerType, Generator>(sample_size, min, max, prng);
        time_point<high_resolution_clock> end = high_resolution_clock::now();
        answer[sample_size] = duration_cast<nanoseconds>(end - begin).count();
    }
    return answer;
}

TimingResults test_all_timings(const std::vector<ArraySize>& sample_sizes, IntegerType min, IntegerType max)
{
    TimingResults answer;
    answer["mt19937"]      = test_prng_timing<std::mt19937>     (sample_sizes, min, max);
    answer["minstd_rand0"] = test_prng_timing<std::minstd_rand0>(sample_sizes, min, max);
    answer["dummy"]        = test_prng_timing<my::DummyPRNG>    (sample_sizes, min, max);
    answer["custom"]       = test_prng_timing<my::CustomPRNG>   (sample_sizes, min, max);
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
        ("output,O", po::value<std::string>()->required(), "csv file to write test timing results, the format is:\n"
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

    std::string sizes_filename = vm["sizes"].as<std::string>();
    std::string output_time_filename = vm["output"].as<std::string>();

    // test chi squared
    {
        std::size_t sample_count = 10;
        std::size_t sample_size = 1'000;
        IntegerType min = std::numeric_limits<IntegerType>::min(), max = std::numeric_limits<IntegerType>::max();
        double significance_level = 0.05;
        test_all_generators(sample_count, sample_size, min, max, significance_level);
        std::cout.flush();
    }

    // test timings
    {
        IntegerType min = std::numeric_limits<IntegerType>::min(), max = std::numeric_limits<IntegerType>::max();
        std::vector<ArraySize> sizes = read_sizes(sizes_filename);

        // csv header
        std::ofstream output(output_time_filename);
        output << "name";
        for (ArraySize size : sizes)
            output << ';' << size;
        output << '\n';

        TimingResults results = test_all_timings(sizes, min, max);
        for (auto& [name, timings] : results)
        {
            std::cerr << std::endl << "Algorithm: " << name << std::endl;
            for (auto [size, time] : timings)
                std::cerr << size << ": " << time << std::endl;
            print_timings_csv_line(output, name, timings);
        }
    }
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
