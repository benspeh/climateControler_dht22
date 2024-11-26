#ifndef PI_POWER_DHT_HPP
#define PI_POWER_DHT_HPP

// Declare the functions
// Function to calculate the saturation vapor pressur
double saturationVaporPressure(double temperature);
// Function to calculate the VPD
double calculateVPD(double temperature, double relativeHumidity);

#endif // PI_POWER_DHT_HPP
