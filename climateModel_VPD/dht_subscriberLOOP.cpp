#define BOOST_ASIO_STANDALONE // Use Boost.Asio standalone mode
#include <iostream>
#include <climateModel_VPD.hpp>
#include <mqtt_client_cpp.hpp>

const std::string HOST = "localhost";
const std::uint16_t PORT = 1883;
const std::string TOPIC = "sensor/dht22";

int main() {
    boost::asio::io_context ioc; // Required for MQTT
    auto client = mqtt::make_sync_client(ioc, HOST, PORT, mqtt::protocol_version::v3_1_1);


    client->set_clean_session(true);
    client->connect();

    client->subscribe(TOPIC, mqtt::qos::at_least_once);

    client->set_publish_handler(
        [](std::uint8_t, mqtt::optional<mqtt::string_view> topic, mqtt::string_view payload) {
            std::cout << "Topic: " << topic.value() << ", Payload: " << payload << std::endl;
        });

    while (true) {
        // Keep the subscriber running
        client->recv();
    }

    return 0;
}
