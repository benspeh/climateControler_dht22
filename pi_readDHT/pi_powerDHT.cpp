#include "bcm2708.hpp"
#include "realtime.hpp"
#include <cstdio>

#include "pi_powerDHT.hpp"

void dht_initPowerPin(int pin) {
    // Set the power pin as an output
    RaspberryPi::pi_mmio_set_output(pin);

    // Power the sensor
    RaspberryPi::pi_mmio_set_high(pin);

    // Delay for sensor stabilization
    busy_wait_milliseconds(1000);

    printf("Sensor power initialized and stabilized.\n");
}

bool is_power_pin_high(int pin) {
    return RaspberryPi::pi_mmio_input(pin) != 0; // Returns true if HIGH
}

void dht_resetPowerPin(int pin) {
  printf("Resetting the sensors powerPin [%d]... \n", pin);

    RaspberryPi::pi_mmio_set_output(pin);

    // Turn off power
    RaspberryPi::pi_mmio_set_low(pin);
    busy_wait_milliseconds(500);

    // Turn on power
    RaspberryPi::pi_mmio_set_high(pin);

    // Verify the pin is correctly set to HIGH
    if (is_power_pin_high(pin)) {
        printf("Power pin is HIGH: Sensor is powered.\n");
    } else {
        printf("Power pin is LOW: Failed to power sensor.\n");
    }

    busy_wait_milliseconds(1000); // Allow sensor to stabilize
}
