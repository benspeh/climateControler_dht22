#include <cerrno>
#include <sys/file.h>  // For flock
#include <fcntl.h>
#include <iostream>
#include <string>
#include <unistd.h>



#include "bcm2708.hpp"
#include "realtime.hpp"
#include "pi_readDHT.hpp"

#define LOCKFILE "/run/lock/dht_read.lck"

// Signal transition timeout in microseconds
#define MAX_WAIT_US 400

// Number of bytes to expect from the DHT sensor.
#define DHT_BYTES  5

// Number of bit pulses to expect from the DHT sensor.
#define DHT_PULSES (1 + DHT_BYTES * 8)

std::string getLogHeader() {
    char buff[64];
    time_t timeNow = time(nullptr);
    struct tm tmNow;
    localtime_r(&timeNow, &tmNow);
    snprintf(buff, sizeof(buff), "%04d-%02d-%02d %02d:%02d:%02d dht_read: ",
             tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday,
             tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec);
    return std::string(buff);
}

#define DHT_READ_LOG(fmt, ...) printf("%s" fmt, getLogHeader().c_str(), ##__VA_ARGS__)

// Return time in microseconds when the pin input signal is changed to the desired state.
static uint32_t getTransitionMicros(int pin, bool transitionHigh) {
    uint32_t expectedValue = transitionHigh ? (1 << pin) : 0;
    uint32_t startedMicros = RaspberryPi::pi_timer_micros();
    while (RaspberryPi::pi_mmio_input(pin) != expectedValue) {
        uint32_t elapsedMicros = RaspberryPi::pi_timer_micros() - startedMicros;
        if (elapsedMicros >= MAX_WAIT_US) {
            return 0; // Timeout
        }
    }
    uint32_t nowMicros = RaspberryPi::pi_timer_micros();
    return (nowMicros == 0) ? UINT32_MAX : nowMicros;
}

static int pi_dht_read(int type, int pin, float* pHumidity, float* pTemperature) {
    *pTemperature = 0.0f;
    *pHumidity = 0.0f;

    uint32_t lowMicros[DHT_PULSES + 1] = {0};
    uint32_t highMicros[DHT_PULSES] = {0};

    RaspberryPi::pi_mmio_set_output(pin);

    // Set process priority for real-time performance.
    set_max_priority();

    RaspberryPi::pi_mmio_set_high(pin);
    sleep_milliseconds(500);

    RaspberryPi::pi_mmio_set_low(pin);
    busy_wait_milliseconds(20);

    RaspberryPi::pi_mmio_set_input(pin);
    RaspberryPi::pi_timer_sleep_micros(2);

    uint32_t lowStartedUs = getTransitionMicros(pin, false);
    if (lowStartedUs == 0) {
        set_default_priority();
        DHT_READ_LOG("Timeout waiting for response low\n");
        return 0;
    }

    int i;
    uint32_t highStartedUs;
    for (i = 0; i < DHT_PULSES; i++) {
        highStartedUs = getTransitionMicros(pin, true);
        if (highStartedUs == 0) {
            set_default_priority();
            DHT_READ_LOG("Timeout waiting for high[%d]\n", i);
            return 0;
        }
        lowMicros[i] = highStartedUs - lowStartedUs;

        lowStartedUs = getTransitionMicros(pin, false);
        if (lowStartedUs == 0) {
            set_default_priority();
            DHT_READ_LOG("Timeout waiting for low[%d]\n", i);
            return 0;
        }
        highMicros[i] = lowStartedUs - highStartedUs;
    }

    highStartedUs = getTransitionMicros(pin, true);
    if (highStartedUs == 0) {
        set_default_priority();
        DHT_READ_LOG("Timeout waiting for high[release]\n");
        return 0;
    }
    lowMicros[DHT_PULSES] = highStartedUs - lowStartedUs;

    set_default_priority();

    uint32_t threshold = 0;
    bool needAdjust = true;
    while (needAdjust) {
        threshold = 0;
        for (i = 1; i < DHT_PULSES; i++) {
            threshold += lowMicros[i];
        }
        threshold /= DHT_PULSES - 1;
        uint32_t lowHighThreshold = threshold * 2;

        needAdjust = false;
        for (i = 1; i < DHT_PULSES; i++) {
            if (highMicros[i] < threshold) {
                uint32_t lowHigh = lowMicros[i] + highMicros[i];
                if (lowHigh >= lowHighThreshold) {
                    highMicros[i] += lowMicros[i] - threshold;
                    lowMicros[i] = threshold;
                    needAdjust = true;
                }
            } else {
                uint32_t lowHigh = highMicros[i] + lowMicros[i + 1];
                if (lowHigh < lowHighThreshold) {
                    highMicros[i] += lowMicros[i + 1] - threshold;
                    lowMicros[i + 1] = threshold;
                    needAdjust = true;
                }
            }
        }
    }

    uint8_t data[DHT_BYTES] = {0};
    for (i = 1; i < DHT_PULSES; i++) {
        int index = (i - 1) / 8;
        data[index] <<= 1;
        if (highMicros[i] >= threshold) {
            data[index] |= 1;
        }
    }

    if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        DHT_READ_LOG("Checksum error\n");
        for (i = 0; i < DHT_PULSES; i++) DHT_READ_LOG("%2d,%4u,%4u\n", i, lowMicros[i], highMicros[i]);
        return 0;
    }

    if (type == DHT11) {
        *pHumidity = static_cast<float>(data[0]);
        *pTemperature = static_cast<float>(data[2]);
    } else if (type == DHT22) {
        *pHumidity = (data[0] * 256 + data[1]) / 10.0f;
        *pTemperature = ((data[2] & 0x7F) * 256 + data[3]) / 10.0f;
        if (data[2] & 0x80) {
            *pTemperature *= -1.0f;
        }
    }
    return 1;
}

static int open_lockfile(const char* filename) {
    int fd = open(filename, O_CREAT | O_RDONLY, 0600);
    if (fd < 0) {
        std::cerr << "Failed to access lock file: " << filename << std::endl;
        return -1;
    }
    if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
        if (errno == EWOULDBLOCK) {
            std::cerr << "Lock file is in use" << std::endl;
        }
        std::perror("Flock failed");
        return -1;
    }
    return fd;
}

static void close_lockfile(int fd) {
    if (flock(fd, LOCK_UN) == -1) {
        std::perror("Failed to unlock file");
    }
    if (close(fd) == -1) {
        std::perror("Closing descriptor on lock file failed");
    }
}

int dht_read(int type, int pin, float* pHumidity, float* pTemperature) {
    int success = 0;
    if (pHumidity == nullptr || pTemperature == nullptr) {
        DHT_READ_LOG("bad argument\n");
    } else if (RaspberryPi::pi_mmio_init() != RaspberryPi::MmioError::SUCCESS) { // 
        DHT_READ_LOG("MMIO init failed. May not be root\n");
    } else {
        int lockfd = -1;
        int count = 10;
        while (count-- > 0) {
            if (lockfd < 0) {
                lockfd = open_lockfile(LOCKFILE);
            }
            if (lockfd >= 0) {
                success = pi_dht_read(type, pin, pHumidity, pTemperature);
                if (success) {
                    count = 0;
                }
            }
            if (count > 0) {
                sleep(1);
            }
        }
        if (lockfd >= 0) {
            close_lockfile(lockfd);
        }
    }
    return success;
}
