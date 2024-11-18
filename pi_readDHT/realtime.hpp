#ifndef REALTIME_H
#define REALTIME_H

#include <cstdint>

// Busy wait delay for accurate timing but high CPU usage.
// Use only for short durations (a few hundred milliseconds at most)!
void busy_wait_milliseconds(uint32_t millis);

// General delay that sleeps to lower CPU usage, though accuracy may be lower.
void sleep_milliseconds(uint32_t millis);

// Increase scheduling priority to attempt 'real-time' performance.
void set_max_priority();

// Revert scheduling priority to normal/default.
void set_default_priority();

#endif // REALTIME_H

