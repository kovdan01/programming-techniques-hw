/**
 * @file
 * @brief Заголовочный файл, содержащий реализацию статистических функций
 * @date Март 2020
*/
#ifndef STAT_UTILS_H
#define STAT_UTILS_H

#include <iterator>
#include <numeric>
#include <cmath>

namespace my
{

namespace stat
{

/**
 * Вычисляет выборочное среднее
 * @tparam Sample тип выборки (например, `std::vector<int>`)
 * @param[in] sample выборка
 * @return выборочное среднее
 */
template <typename Sample>
double mean(const Sample& sample)
{
    return static_cast<double>(std::accumulate(std::begin(sample), std::end(sample), 0.0)) / std::size(sample);
}

/**
 * Вычисляет среднеквадратичное отклонение выборки
 * @tparam Sample тип выборки (например, `std::vector<int>`)
 * @param[in] sample выборка
 * @return среднеквадратичное отклонение
 */
template <typename Sample>
double variance(const Sample& sample)
{
    double average = mean(sample);
    double sum = 0;
    for (const auto& value : sample)
        sum += (value - average) * (value - average);
    sum /= std::size(sample);
    return std::sqrt(sum);
}

/**
 * Вычисляет коэффициент вариации выборки
 * @tparam Sample тип выборки (например, `std::vector<int>`)
 * @param[in] sample выборка
 * @return коэффициент вариации
 */
template <typename Sample>
double variability(const Sample& sample)
{
    return variance(sample) / mean(sample);
}

} // namespace stat

} // namespace my

#endif // STAT_UTILS_H
