#include <cerrno>
#include <cstring>
#include <sched.h>
#include <sys/time.h>
#include <time.h>

#include "realtime.hpp"

void busy_wait_milliseconds(uint32_t millis) {
    // Set delay time period.
    struct timeval deltatime;
    deltatime.tv_sec = millis / 1000;
    deltatime.tv_usec = (millis % 1000) * 1000;
    struct timeval walltime;
    // Get current time and add delay to find end time.
    gettimeofday(&walltime, nullptr);
    struct timeval endtime;
    timeradd(&walltime, &deltatime, &endtime);
    // Tight loop to waste time (and CPU) until enough time has elapsed.
    while (timercmp(&walltime, &endtime, <)) {
        gettimeofday(&walltime, nullptr);
    }
}

void sleep_milliseconds(uint32_t millis) {
    struct timespec sleep;
    sleep.tv_sec = millis / 1000;
    sleep.tv_nsec = (millis % 1000) * 1000000L;
    while (clock_nanosleep(CLOCK_MONOTONIC, 0, &sleep, &sleep) && errno == EINTR);
}

void set_max_priority() {
    struct sched_param sched;
    std::memset(&sched, 0, sizeof(sched));
    // Use FIFO scheduler with the highest priority for minimal kernel context switching.
    sched.sched_priority = sched_get_priority_max(SCHED_FIFO);
    sched_setscheduler(0, SCHED_FIFO, &sched);
}

void set_default_priority() {
    struct sched_param sched;
    std::memset(&sched, 0, sizeof(sched));
    // Revert to default scheduler with priority 0.
    sched.sched_priority = 0;
    sched_setscheduler(0, SCHED_OTHER, &sched);
}

