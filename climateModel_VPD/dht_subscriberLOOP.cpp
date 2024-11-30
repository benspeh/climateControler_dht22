#define BOOST_ASIO_STANDALONE // Use Boost.Asio standalone mode
#include <iostream>
#include "climateModel_VPD.hpp"
#include <mqtt_client_cpp.hpp>
#include <string>

const std::string HOST = "127.0.0.1";  // Replace with your broker's IP
const std::uint16_t PORT = 1883;
const std::string TOPIC = "sensor/dht22";

int main() {
    try {
        boost::asio::io_context ioc; // Required for MQTT
        auto client = mqtt::make_sync_client(ioc, HOST, PORT, mqtt::protocol_version::v3_1_1);

        client->set_clean_session(true); // Clean session ensures no persistent state

        // Set publish handler to handle incoming messages
        client->set_publish_handler(
            [](boost::optional<std::uint16_t> packet_id, mqtt::publish_options options, mqtt::buffer topic, mqtt::buffer payload) {
                std::cout << "Received message on topic: " << std::string(topic.begin(), topic.end())
                          << ", Payload: " << std::string(payload.begin(), payload.end()) << std::endl;
                return true; // Indicate that the message was successfully handled
            });

        // Connect to the MQTT broker
        client->connect();

        // Subscribe to the desired topic
        client->subscribe(TOPIC, mqtt::qos::at_least_once);

        while (true) {
            // Process incoming messages
            client->process_message(); // Blocks until a message is received or timeout occurs
        }

        // Disconnect when finished (although we won't reach here in the infinite loop)
        client->disconnect();
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

