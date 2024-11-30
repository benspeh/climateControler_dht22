#define BOOST_ASIO_STANDALONE // Use Boost.Asio standalone mode
#include <iostream>
#include <string>
#include <mqtt_client_cpp.hpp>
#include "pi_readDHT.hpp"
#include "pi_powerDHT.hpp"

// Default GPIO dataPin and sleep time if not specified
#define DEFAULT_POWER_PIN 22
#define DEFAULT_DATA_PIN 23
#define DEFAULT_SLEEP_TIME 60

// MQTT settings
const std::string HOST = "127.0.0.1";
const std::uint16_t PORT = 1883;
const std::string TOPIC = "sensor/dht22";

int main(int argc, const char **argv) {
    int powerPin = DEFAULT_POWER_PIN;
    int dataPin = DEFAULT_DATA_PIN;
    int sleep_time = DEFAULT_SLEEP_TIME;

    if (argc > 1) powerPin = atoi(argv[1]);
    if (argc > 2) dataPin = atoi(argv[2]);
    if (argc > 3) sleep_time = atoi(argv[3]);

    setvbuf(stdout, NULL, _IOLBF, 0);
    std::cout << "Starting DHT sensor readings on GPIO dataPin " << dataPin 
              << " with a " << sleep_time << " second interval." << std::endl;

    float humidity, temperature;
    dht_initPowerPin(powerPin);

    // Create MQTT client
    boost::asio::io_context ioc; // Required for MQTT
    auto client = mqtt::make_sync_client(ioc, HOST, PORT, mqtt::protocol_version::v3_1_1);

    client->set_clean_session(true);
    client->connect();

    while (true) {
        int success = dht_read(AM2302, dataPin, &humidity, &temperature);
        std::ostringstream payload;

        if (success) {
            payload << "Temperature: " << temperature << "°C, Humidity: " << humidity << "%";
            std::cout << payload.str() << std::endl;
        } else {
            payload << "Failed to read sensor!";
            std::cout << payload.str() << std::endl;
        }

        client->publish(TOPIC, payload.str());
        sleep(sleep_time);
    }

    client->disconnect();
    return 0;
}
