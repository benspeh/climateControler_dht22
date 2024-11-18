#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // for sleep()
#include "pi_readDHT.hpp"

// GPIO pin number for DHT sensor
#define DHTPIN 24

int main(int argc, const char **argv) {
    float humidity, temperature;
    int count = argc < 2 ? 1 : atoi(argv[1]);  // Default 1 read if no argument is given
    while (count-- > 0) {
        int success = Raspberry::dht_read(AM2302, DHTPIN, &humidity, &temperature);
        if (success) {
            printf("Temperature: %.1f°C  Humidity: %.1f%%\n", temperature, humidity);
        } else {
            printf("Failed to read sensor!\n");
        }
        printf("Wait 1min...\n");
        sleep(60);  // Wait 60 seconds between reads
    }

    return 0;
}
