// subscriber.cpp

#define BOOST_ASIO_STANDALONE
#include <iostream>
#include <string>
#include <mqtt_client_cpp.hpp>
#include "climateModel_VPD.hpp"  // Include your model header if you need to process the data

// MQTT settings
const std::string HOST = "0.0.0.0"; // NanoMQ broker IP
const std::uint16_t PORT = 1883;
const std::string TOPIC = "sensor/dht22";

int main() {
    try {
        boost::asio::io_context ioc; // Required for MQTT
        auto client = mqtt::make_sync_client(ioc, HOST, PORT, mqtt::protocol_version::v3_1_1);

        client->set_clean_session(true);

        // Set publish handler to handle incoming messages
        client->set_publish_handler(
            [](boost::optional<std::uint16_t> packet_id, mqtt::publish_options options, mqtt::buffer topic, mqtt::buffer payload) {
                std::cout << "Received message on topic: " 
                          << std::string(topic.begin(), topic.end()) 
                          << ", Payload: " << std::string(payload.begin(), payload.end()) << std::endl;
                return true; // Indicate the message was successfully handled
            });

        // Connect to the server (NanoMQ)
        client->connect();

        // Subscribe to the topic
        client->subscribe(TOPIC, mqtt::qos::at_least_once);

        // Run the IO context to process messages
        ioc.run(); // Blocks until all asynchronous handlers are processed

        // Disconnect when finished (though we won't reach here as `ioc.run()` blocks indefinitely)
        client->disconnect();
    } catch (const std::exception &ex) {
        std::cerr << "Subscriber error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
