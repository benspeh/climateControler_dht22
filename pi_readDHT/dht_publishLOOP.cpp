#define BOOST_ASIO_STANDALONE
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <mqtt_client_cpp.hpp>
#include "pi_readDHT.hpp"  // Include your own DHT sensor reading header
#include "pi_powerDHT.hpp" // Include your own DHT power management header

// Default GPIO dataPin and sleep time if not specified
#define DEFAULT_POWER_PIN 22
#define DEFAULT_DATA_PIN 23
#define DEFAULT_SLEEP_TIME 60

// MQTT settings
const std::string HOST = "0.0.0.0"; // Localhost for testing
const std::uint16_t PORT = 1883;
const std::string TOPIC = "sensor/dht22";

int main(int argc, const char **argv) {
    int powerPin = DEFAULT_POWER_PIN;
    int dataPin = DEFAULT_DATA_PIN;
    int sleep_time = DEFAULT_SLEEP_TIME;

    if (argc > 1) powerPin = atoi(argv[1]);
    if (argc > 2) dataPin = atoi(argv[2]);
    if (argc > 3) sleep_time = atoi(argv[3]);

    //setvbuf(stdout, NULL, _IOLBF, 0);
    std::cout << "Starting DHT sensor readings on GPIO dataPin " << dataPin 
              << " with a " << sleep_time << " second interval." << std::endl;

    float humidity, temperature;
    dht_initPowerPin(powerPin);

    // Create MQTT client (header-only mode)
    boost::asio::io_context ioc;
    auto client = mqtt::make_sync_client(ioc, HOST, PORT, mqtt::protocol_version::v3_1_1);

    client->set_clean_session(true);
    client->set_auto_pub_response(false); // Avoids unnecessary responses in peer-to-peer mode

    try {
        client->connect();

        while (true) {
            int success = dht_read(AM2302, dataPin, &humidity, &temperature);
          
            std::ostringstream payload;

             if (success) {
                payload << temperature << "," << humidity; // Format as CSV
                std::cout << "Publishing: " << payload.str() << std::endl;
                client->publish(TOPIC, payload.str(), mqtt::qos::at_least_once);
            } else {
                dht_resetPowerPin(powerPin);
            
                success = dht_read(AM2302, dataPin, &humidity, &temperature);
                     if (success) {
                        payload << temperature << "," << humidity; // Format as CSV
                        std::cout << "Publishing: " << payload.str() << std::endl;
                        client->publish(TOPIC, payload.str(), mqtt::qos::at_least_once);
                    } else {
                        payload << "Failed to read sensor after resetting powerPin";
                        std::cout << payload.str() << std::endl;
                    }
            }

            // Publish the message to the topic with retained flag set to true
            client->publish(TOPIC, payload.str());

            // Sleep for the configured interval
            std::this_thread::sleep_for(std::chrono::seconds(sleep_time)); // Publish interval
        }

        client->disconnect();
    } catch (const std::exception &ex) {
        std::cerr << "Publisher error: " << ex.what() << std::endl;
    }

    return 0;
}
