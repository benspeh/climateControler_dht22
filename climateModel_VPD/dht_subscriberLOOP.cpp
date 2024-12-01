#define BOOST_ASIO_STANDALONE
#include <mqtt_client_cpp.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <memory>
#include <mutex>

const std::string HOST = "0.0.0.0";  // Broker address
const uint16_t PORT = 1883;           // Broker port
const std::string TOPIC = "sensor/dht22";

// Mutex for synchronizing access to the client in multiple threads
std::mutex client_mutex;

template <typename ClientType>
void monitor_connection(std::shared_ptr<ClientType> client) {
    while (true) {
        try {
            // Lock the mutex to ensure thread-safe access to the client
            std::lock_guard<std::mutex> lock(client_mutex);
            
            // Check if the client is disconnected
            if (!client->connected()) {
                std::cerr << "Disconnected from broker. Reconnecting..." << std::endl;

                // Attempt to reconnect (keep the existing client instance)
                try {
                    client->connect();  // Reconnect by calling connect again
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

        std::this_thread::sleep_for(std::chrono::seconds(5)); // Check every 5 seconds
    }
}

int main() {
    try {
        boost::asio::io_context ioc;

        // Create an MQTT client (using a shared_ptr)
        auto client = mqtt::make_sync_client(ioc, HOST, PORT);

        // Set clean session
        client->set_clean_session(true);

        // Define message handler
        client->set_publish_handler(
            [&](mqtt::optional<std::uint16_t> packet_id,
                mqtt::publish_options pub_opts, 
                mqtt::buffer topic,
                mqtt::buffer payload) -> bool {
                try {
                    std::cout << "Received message on topic: " 
                              << topic.to_string() 
                              << ", payload: " 
                              << payload.to_string() 
                              << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error in publish handler: " << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "Unknown error in publish handler" << std::endl;
                }
                return true;
            });

        // Connect to the broker
        client->connect();  
        std::cerr << "Connected to broker" << std::endl;

        // Subscribe to the topic
        client->subscribe(TOPIC, mqtt::qos::at_most_once);  
        std::cerr << "Subscribed to topic: " << TOPIC << std::endl;

        // Start a thread to monitor connection state (using std::thread)
        std::thread connection_monitor_thread([&]() { monitor_connection(client); });
        connection_monitor_thread.detach();  // Detach to allow continuous monitoring

        // Run the client
        std::cerr << "Entering io_context.run()" << std::endl;
        ioc.run();  // This should block until the program is terminated

    } catch (const std::exception& e) {
        std::cerr << "Error in main: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error in main" << std::endl;
    }

    return 0;
}

