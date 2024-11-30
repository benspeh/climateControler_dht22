#include "cimateModel_VPD.hpp"

// Function to calculate the saturation vapor pressure (e_s) at a given temperature
double saturationVaporPressure(double temperature) {
    return 0.6108 * exp((17.27 * temperature) / (temperature + 237.3));
}

// Function to calculate the VPD
double calculateVPD(double temperature, double relativeHumidity) {
    // Calculate saturation vapor pressure
    double e_s = saturationVaporPressure(temperature);

    // Calculate actual vapor pressure
    double e_a = e_s*(relativeHumidity/100.0);

//    printf("saturatrionVP: %f\n", e_s);
//    printf("relative saturatrionVP: %f\n", e_a);

    // Calculate VPD
    double vpd = e_s - e_a;
    return vpd;
}

