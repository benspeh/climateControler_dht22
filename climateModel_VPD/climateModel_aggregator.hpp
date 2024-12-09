#ifndef CLIMATEMODEL_AGGREGATOR_HPP
#define CLIMATEMODEL_AGGREGATOR_HPP

#include <vector>
#include <numeric>  // for std::accumulate
#include <chrono>
#include <ctime>
#include <iostream>

class MeanAggregator {
public:
    // Add a new value with timestamp
    void addValue(double value, const std::time_t& timestamp);

    // Compute hourly and daily mean
    double hourlyMean() const;
    double dailyMean() const;

    // Reset data for new hour or day
    void resetHourly();
    void resetDaily();

private:
    std::vector<double> hourlyData;
    std::vector<double> dailyData;
};

class SumAggregator {
public:
    // Add a new value with timestamp
    void addValue(double value, const std::time_t& timestamp);

    // Compute daily and total sum
    double dailySum() const;
    double totalSum() const;

    // Reset data for new day
    void resetDaily();

private:
    std::vector<double> dailyData;
    double total = 0.0;
};

#endif // CLIMATEMODEL_AGGREGATOR_HPP
