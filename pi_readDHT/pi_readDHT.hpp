#ifndef PI_DHT_READ_HPP
#define PI_DHT_READ_HPP

namespace Raspberry {
  // Define sensor types
  #define DHT11 11
  #define DHT22 22
  #define AM2302 22
  
  /**
   * Read humidity and temperature from the Adafruit DHT sensor, with retries.
   *
   * @param type Sensor type (e.g., AM2302)
   * @param pin GPIO pin number (e.g., 4)
   * @param pHumidity Pointer to a float where humidity is set on return
   * @param pTemperature Pointer to a float where temperature is set on return
   * @return 1 if successful, 0 if failed
   */
  int dht_read(int type, int pin, float* pHumidity, float* pTemperature);
}
#endif  // PI_DHT_READ_HPP
