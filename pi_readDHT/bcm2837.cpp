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

#include "bcm2708.hpp"

#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

namespace RaspberryPi {

// Constants for the memory-mapped registers on the BCM2837
constexpr uintptr_t BASE = 0x3F000000;
constexpr uintptr_t GPIO_BASE = BASE + 0x200000;
constexpr size_t GPIO_LENGTH = 4096;
constexpr uintptr_t BCM2708_ST_BASE = BASE + 0x3000; // System Timer base address

// GPIO and Timer memory-mapped pointers
volatile uint32_t* pi_mmio_gpio = nullptr;
volatile uint32_t* pi_mmio_timer = nullptr;

// Function to initialize GPIO and Timer memory-mapped I/O
MmioError pi_mmio_init() {
    if (!pi_mmio_gpio) {
        // Open /dev/mem with read/write and sync flags
        int fd = open("/dev/mem", O_RDWR | O_SYNC);
        if (fd == -1) {
            // Failed to open /dev/mem, likely due to lack of permissions
            return MmioError::DEVMEM_OPEN_FAIL;
        }

        // Map GPIO memory to process space
        pi_mmio_gpio = reinterpret_cast<uint32_t*>(
            mmap(nullptr, GPIO_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE)
        );

        // Map System Timer register to process space
        pi_mmio_timer = reinterpret_cast<uint32_t*>(
            mmap(nullptr, GPIO_LENGTH, PROT_READ, MAP_SHARED, fd, BCM2708_ST_BASE)
        );

        close(fd); // Close the file descriptor after mapping
        if (pi_mmio_gpio == MAP_FAILED || pi_mmio_timer == MAP_FAILED) {
            // In case of failure, reset pointers and return error
            pi_mmio_gpio = pi_mmio_timer = nullptr;
            return MmioError::MMAP_FAIL;
        }
    }
    return MmioError::SUCCESS;
}

// Function to sleep for a specified number of microseconds using the system timer
void pi_timer_sleep_micros(uint32_t micros) {
    uint32_t start = pi_timer_micros();
    uint32_t elapsed = 0;
    while (elapsed < micros) {
        elapsed = pi_timer_micros() - start;
    }
}

} // namespace RaspberryPi
