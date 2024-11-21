#ifndef DHT_LOGGING_HPP
#define DHT_LOGGING_HPP

#include <cstdio>      // printf, snprintf
#include <ctime>       // time_t, time, localtime_r
#include <string>      // std::string

// Function to generate a timestamped log header
inline std::string getLogHeader() {
    char buff[64];
    time_t timeNow = time(nullptr);
    struct tm tmNow;
    localtime_r(&timeNow, &tmNow);
    snprintf(buff, sizeof(buff), "%04d-%02d-%02d %02d:%02d:%02d dht_read: ",
             tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday,
             tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);
    return std::string(buff);
}

// Macro to log messages with the header
#define DHT_READ_LOG(fmt, ...) printf("%s" fmt, getLogHeader().c_str(), ##__VA_ARGS__)

#endif // DHT_LOGGING_HPP
