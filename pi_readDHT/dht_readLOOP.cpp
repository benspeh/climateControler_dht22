#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep()
#include "pi_readDHT.hpp"

// Default GPIO pin and sleep time if not specified
#define DEFAULT_PIN 24
#define DEFAULT_SLEEP_TIME 60

int main(int argc, const char **argv) {
    int pin = DEFAULT_PIN;
    int sleep_time = DEFAULT_SLEEP_TIME;
    
    // Parse GPIO pin and sleep time from command line arguments
    if (argc > 1) {
        pin = atoi(argv[1]);
    }
    if (argc > 2) {
        sleep_time = atoi(argv[2]);
    }

    printf("Starting DHT sensor readings on GPIO pin %d with a %d second interval.\n", pin, sleep_time);
    
    float humidity, temperature;
    
    while (true) {  // Infinite loop
        int success = dht_read(AM2302, pin, &humidity, &temperature);
        
        if (success) {
            printf("Temperature: %.1f°C  Humidity: %.1f%%\n", temperature, humidity);
        } else {
            printf("Failed to read sensor!\n");
        }
        
        printf("Waiting %d seconds before the next read...\n", sleep_time);
        sleep(sleep_time);  // Wait for the specified interval
    }

    return 0;
}

