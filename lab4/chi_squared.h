/**
 * @file
 * @brief Заголовочный файл, содержащий реализацию проверки критерия
 * хи-квадрат Пирсона на принадлежность равномерному распределению
 * @date Март 2020
*/
#ifndef CHI_SQUARED_H
#define CHI_SQUARED_H

#include <boost/math/distributions.hpp>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <utility>
#include <sstream>

namespace my
{

/// Допустимые типы распределений
enum class DistributionType
{
    UNIFORM_INT,  ///< равномерное целочисленное
    UNIFORM_REAL, ///< равномерное непрерывное
};

/**
 * Вычисляет значение хи-квадрат для данной выборки
 * @tparam Number тип элемента выборки
 * @param[in] sample выборка
 * @param[in] min минимальное значение, которое могло быть получено при генерации выборки
 * @param[in] max максимальное значение, которое могло быть получено при генерации выборки
 * @param[in] type тип распределения
 * @return `std::pair`, первый элемент которой - посчитанное значение хи-квадрат,
 * второй - количество степеней свободы
 */
template <typename Number>
std::pair<double, std::size_t> chi_squared(const std::vector<Number>& sample, Number min, Number max, DistributionType type)
{
    std::ostringstream exception_message;
    if (min >= max)
    {
        exception_message << "Range minimum " << min << " is not less than range maximum " << max;
        throw std::invalid_argument(exception_message.str());
    }
    if (sample.size() < 2)
    {
        exception_message << "Sample size " << sample.size() << " is too small";
        throw std::invalid_argument(exception_message.str());
    }

    int add = (type == DistributionType::UNIFORM_INT ? 1 : 0);
    double range_size = static_cast<double>(max) - min + add;

    std::size_t interval_count = static_cast<std::size_t>(1 + std::log2(sample.size()));
    if (type == DistributionType::UNIFORM_INT && static_cast<double>(interval_count) > range_size)
        interval_count = static_cast<std::size_t>(range_size);
    double interval_size = range_size / interval_count;

    std::vector<Number> interval_edges(interval_count + 1);
    interval_edges.front() = min;
    for (std::size_t i = 1; i + 1 < interval_edges.size(); ++i)
        interval_edges[i] = static_cast<Number>(min + interval_size * i);
    interval_edges.back() = max;

    std::vector<double> interval_probabilities(interval_count);
    for (std::size_t i = 0; i + 1 < interval_probabilities.size(); ++i)
        interval_probabilities[i] = (interval_edges[i + 1] - interval_edges[i]) / range_size;
    interval_probabilities.back() = (interval_edges[interval_count] - interval_edges[interval_count - 1] + add) / range_size;

    std::vector<double> emperical_probabilities(interval_count, 0);
    for (const Number& value : sample)
    {
        std::size_t index = std::upper_bound(interval_edges.begin(), interval_edges.end(), value) - interval_edges.begin() - 1;
        if (index == interval_count && type == DistributionType::UNIFORM_INT && value == max)
        {
            ++emperical_probabilities.back();
            continue;
        }
        if (index == interval_count)
        {
            exception_message << "Sample value " << value << " is not in range [" << min << "; " << max
                              << (type == DistributionType::UNIFORM_INT ? "]" : ")");
            throw std::invalid_argument(exception_message.str());
        }
        ++emperical_probabilities[index];
    }
    std::for_each(emperical_probabilities.begin(), emperical_probabilities.end(), [&sample](double& value){ value /= sample.size(); });

    double result = 0;
    for (std::size_t i = 0; i < interval_count; ++i)
    {
        double numerator = emperical_probabilities[i] - interval_probabilities[i];
        result += numerator * numerator / interval_probabilities[i];
    }
    result *= sample.size();

    return { result, interval_count - 1 };
}

/**
 * Проверяет статистический критерий хи-квадрат Пирсона для заданной выборки
 * @tparam Number тип элемента выборки
 * @param[in] sample выборка
 * @param[in] min минимальное значение, которое могло быть получено при генерации выборки
 * @param[in] max максимальное значение, которое могло быть получено при генерации выборки
 * @param[in] significance_level уровень значимости (вероятность отклонить истинную гипотезу)
 * @param[in] type тип распределения
 * @return `true`, если выборка удовлетворяет критерию хи-квадрат, `false` в противном случае
 */
template <typename Number>
bool check_chi_squared_criteria(const std::vector<Number>& sample, Number min, Number max, double significance_level, DistributionType type)
{
    if (significance_level <= 0 || significance_level >= 1)
    {
        std::ostringstream exception_message;
        exception_message << "Significance level " << significance_level << " is not in (0; 1) interval";
        throw std::invalid_argument(exception_message.str());
    }
    namespace bm = boost::math;
    auto [chi_squared_value, degrees_of_freedom] = chi_squared<Number>(sample, min, max, type);
    return (chi_squared_value <= bm::quantile(bm::chi_squared_distribution<double>(degrees_of_freedom), 1 - significance_level));
}

} // namespace my

#endif // CHI_SQUARED_H
