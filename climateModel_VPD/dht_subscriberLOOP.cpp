#include <iostream>
#include <mqtt_client_cpp.hpp>

const std::string HOST = "localhost";
const std::string TOPIC = "sensor/dht22";

int main() {
    auto client = mqtt::make_sync_client(HOST, "1883");

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
