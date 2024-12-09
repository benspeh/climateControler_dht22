#include "climateModel_aggregator.hpp"

// ------------------------ MeanAggregator ------------------------

void MeanAggregator::addValue(double value, const std::time_t& timestamp) {
    // Add value to both hourly and daily data
    hourlyData.push_back(value);
    dailyData.push_back(value);
}

double MeanAggregator::hourlyMean() const {
    if (hourlyData.empty()) return 0.0;
    return std::accumulate(hourlyData.begin(), hourlyData.end(), 0.0) / hourlyData.size();
}

double MeanAggregator::dailyMean() const {
    if (dailyData.empty()) return 0.0;
    return std::accumulate(dailyData.begin(), dailyData.end(), 0.0) / dailyData.size();
}

void MeanAggregator::resetHourly() {
    hourlyData.clear();
}

void MeanAggregator::resetDaily() {
    dailyData.clear();
}

// ------------------------ SumAggregator ------------------------

void SumAggregator::addValue(double value, const std::time_t& timestamp) {
    // Add value to daily data and update total sum
    dailyData.push_back(value);
    total += value;
}

double SumAggregator::dailySum() const {
    return std::accumulate(dailyData.begin(), dailyData.end(), 0.0);
}

double SumAggregator::totalSum() const {
    return total;
}

void SumAggregator::resetDaily() {
    dailyData.clear();
}
