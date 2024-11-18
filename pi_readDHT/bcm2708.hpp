// Adapted to C++ by Benjamin Spehle
// Copyright (c) 2014 Adafruit Industries
// Author: Tony DiCola
// Based on code from Gert van Loo & Dom: http://elinux.org/RPi_Low-level_peripherals#GPIO_Code_examples

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef BCM2708_HPP
#define BCM2708_HPP

#include <cstdint>

namespace RaspberryPi {

// Error codes
enum class MmioError {
    SUCCESS,
    DEVMEM_OPEN_FAIL,
    MMAP_FAIL,
};

// Pointers to GPIO and Timer memory-mapped regions
extern volatile uint32_t* pi_mmio_gpio;
extern volatile uint32_t* pi_mmio_timer;

// Initialize GPIO and Timer memory mapping
MmioError pi_mmio_init();

// Inline functions for GPIO manipulation
inline void pi_mmio_set_input(const int gpio_number) {
    // Set GPIO register to 000 for specified GPIO number.
    *(pi_mmio_gpio + (gpio_number / 10)) &= ~(7 << ((gpio_number % 10) * 3));
}

inline void pi_mmio_set_output(const int gpio_number) {
    // First set to 000 using input function.
    pi_mmio_set_input(gpio_number);
    // Next set bit 0 to 1 to set output.
    *(pi_mmio_gpio + (gpio_number / 10)) |= (1 << ((gpio_number % 10) * 3));
}

inline void pi_mmio_set_high(const int gpio_number) {
    *(pi_mmio_gpio + 7) = 1 << gpio_number;
}

inline void pi_mmio_set_low(const int gpio_number) {
    *(pi_mmio_gpio + 10) = 1 << gpio_number;
}

inline uint32_t pi_mmio_input(const int gpio_number) {
    return *(pi_mmio_gpio + 13) & (1 << gpio_number);
}

// Function to retrieve the current microsecond timestamp from the system timer
inline uint32_t pi_timer_micros() {
    return pi_mmio_timer[1];
}

// Function to sleep for a specified number of microseconds
void pi_timer_sleep_micros(uint32_t micros);

} // namespace RaspberryPi

#endif // BCM2708_HPP
