#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep()
#include "pi_readDHT.hpp"
#include "pi_powerDHT.hpp"



// Default GPIO dataPin and sleep time if not specified
#define DEFAULT_POWER_PIN 22
#define DEFAULT_DATA_PIN 23

#define DEFAULT_SLEEP_TIME 60

int main(int argc, const char **argv) {
    int powerPin = DEFAULT_POWER_PIN;        
    int dataPin = DEFAULT_DATA_PIN;

    int sleep_time = DEFAULT_SLEEP_TIME;
    
    // Parse GPIO dataPin and sleep time from command line arguments
    if (argc > 1) {
        powerPin = atoi(argv[1]);
    }
    if (argc > 2) {
        dataPin = atoi(argv[2]);
    }
    if (argc > 3) {
        sleep_time = atoi(argv[3]);
    }
    
    setvbuf(stdout, NULL, _IOLBF, 0); // Set stdout to line-buffered mode
    printf("Starting DHT sensor readings on GPIO dataPin %d with a %d second interval.\n", dataPin, sleep_time);
    
    float humidity, temperature;

    dht_initPowerPin(powerPin);
    
    while (true) {  // Infinite loop
        int success = dht_read(AM2302, dataPin, &humidity, &temperature);
        
        if (success) {
            printf("Temperature: %.1f°C  Humidity: %.1f%%\n", temperature, humidity);
        } else {
            printf("Failed to read sensor!\n");
            dht_resetPowerPin(powerPin);
            success = dht_read(AM2302, dataPin, &humidity, &temperature);
            if (success) {
                printf("Temperature: %.1f°C  Humidity: %.1f%%\n", temperature, humidity);
            } else {
                printf("Failed to read sensor after reset powerPin[%d]!\n", powerPin);
            }
        }
        
        printf("Waiting %d seconds before the next read...\n", sleep_time);
        fflush(stdout); // Ensure the log appears immediately
        sleep(sleep_time);  // Wait for the specified interval
    }

    return 0;
}

