#include <stdio.h>
#include <stdlib.h>
#include "pi_dht_read.h"

// GPIO pin number for DHT sensor
#define DHTPIN 24

int main(int argc, const char **argv) {
        float humidity, temperature;
        int count = argc < 2 ? 1 : atoi(argv[1]);
        while (count-- > 0) {
                int success = dht_read(AM2302, DHTPIN, &humidity, &temperature);
                if (success) {
                        printf("temperature:%.1f Humidity:%.1f\n", temperature, humidity);
                } else {
                printf("Failed to read sensor!\n");
                }
        printf("Wait 1min...\n");
        sleep(60);
        }

        return 0;
}

