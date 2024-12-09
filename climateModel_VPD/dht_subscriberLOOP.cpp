#define BOOST_ASIO_STANDALONE
#include <mqtt_client_cpp.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include <memory>
#include <mutex>
#include <sstream>
#include "climateModel_VPD.hpp"
#include "climateModel_aggregator.hpp" // Include the aggregator header

const std::string HOST = "0.0.0.0";  // Broker address
const uint16_t PORT = 1883;          // Broker port
const std::string TOPIC = "sensor/dht22";

// Mutex for synchronizing access to the client in multiple threads
std::mutex client_mutex;

// Aggregators for temperature and humidity
MeanAggregator temperatureAggregator;
MeanAggregator humidityAggregator;
MeanAggregator vpdAggregator;
// Helper variables for reset conditions
int lastHour = -1;
int lastDay = -1;
    
    // Helper function to reset hourly statistics if needed
bool hourlyAggregatorReset() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    if (lastHour == -1) {
        // Initialization
        lastHour = localTime->tm_hour;
        std::cout << "Hourly aggregator initialized!" << std::endl;
    } else if (localTime->tm_hour != lastHour) {
        // Hourly reset triggered
        std::cout << "End of Hour " << lastHour << " Report:" << std::endl;
        std::cout << "Hourly Mean Temperature: " << temperatureAggregator.hourlyMean() << "°C" << std::endl;
        std::cout << "Hourly Mean Humidity: " << humidityAggregator.hourlyMean() << "%" << std::endl;
        std::cout << "Hourly Mean VPD: " << vpdAggregator.hourlyMean() << " kPa" << std::endl;

        // Reset values
        lastHour = localTime->tm_hour;
        temperatureAggregator.resetHourly();
        humidityAggregator.resetHourly();
        vpdAggregator.resetHourly();

        return true; // Indicate reset occurred
    }

    return false; // No reset needed
}


// Helper function to reset daily statistics if needed
// Helper function to reset daily statistics if needed
bool dailyAggregatorReset() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    static int lastDay = -1; // Track the last day

    if (lastDay == -1) {
        // Initialization
        lastDay = localTime->tm_mday;
        std::cout << "Daily aggregator initialized!" << std::endl;
    } else if (localTime->tm_mday != lastDay) {
        // Daily reset triggered
        std::cout << "End of Day " << lastDay << " Report:" << std::endl;
        std::cout << "Daily Mean Temperature: " << temperatureAggregator.dailyMean() << "°C" << std::endl;
        std::cout << "Daily Mean Humidity: " << humidityAggregator.dailyMean() << "%" << std::endl;
        std::cout << "Daily Mean VPD: " << vpdAggregator.dailyMean() << " kPa" << std::endl;

        // Reset values
        lastDay = localTime->tm_mday;
        temperatureAggregator.resetDaily();
        humidityAggregator.resetDaily();
        vpdAggregator.resetDaily();

        return true; // Indicate reset occurred
    }

    return false; // No reset needed
}

// MQTT message handler
void messageHandler(const std::string& topic, const std::string& payload) {
    try {
        std::cout << "Received message on topic: " << topic 
                  << ", payload: " << payload << std::endl;

        // Parse CSV payload
        std::istringstream payloadStream(payload);
        std::string token;
        double temperature = 0.0;
        double humidity = 0.0;

        if (std::getline(payloadStream, token, ',')) {
            temperature = std::stod(token); // Extract temperature
        }
        if (std::getline(payloadStream, token, ',')) {
            humidity = std::stod(token); // Extract humidity
        }

        // Calculate and print VPD
        double vpd = calculateVPD(temperature, humidity);
        
        std::cout << "Temperature: " << temperature 
                  << "°C, Humidity: " << humidity 
                  << "%, VPD: " << vpd << " kPa" << std::endl;

        // Add data to aggregators
        std::time_t now = std::time(nullptr);
        temperatureAggregator.addValue(temperature, now);
        humidityAggregator.addValue(humidity, now);
        vpdAggregator.addValue(vpd, now);

        // Example: Print hourly and daily statistics
        hourlyAggregatorReset();
        dailyAggregatorReset();

    } catch (const std::exception& e) {
        std::cerr << "Error processing message: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error processing message" << std::endl;
    }
}

template <typename ClientType>
void monitor_connection(std::shared_ptr<ClientType> client) {
    while (true) {
        try {
            std::lock_guard<std::mutex> lock(client_mutex);
            if (!client->connected()) {
                std::cerr << "Disconnected from broker. Reconnecting..." << std::endl;
                try {
                    client->connect();
                    std::cerr << "Reconnected to broker." << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Reconnection failed: " << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "Unknown error during reconnection" << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error in connection monitor: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error in connection monitor" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main() {
    try {
        boost::asio::io_context ioc;

        auto client = mqtt::make_sync_client(ioc, HOST, PORT);
        client->set_clean_session(true);

        client->set_publish_handler(
            [&](mqtt::optional<std::uint16_t> packet_id,
                mqtt::publish_options pub_opts, 
                mqtt::buffer topic,
                mqtt::buffer payload) -> bool {
                messageHandler(topic.to_string(), payload.to_string());
                return true;
            });

        client->connect();
        std::cerr << "Connected to broker" << std::endl;

        client->subscribe(TOPIC, mqtt::qos::at_most_once);
        std::cerr << "Subscribed to topic: " << TOPIC << std::endl;

        std::thread connection_monitor_thread([&]() { monitor_connection(client); });
        connection_monitor_thread.detach();

        std::cerr << "Entering io_context.run()" << std::endl;
        ioc.run();

    } catch (const std::exception& e) {
        std::cerr << "Error in main: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error in main" << std::endl;
    }

    return 0;
}
