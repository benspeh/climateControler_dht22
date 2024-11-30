#ifndef CLIMATEMODEL_VPD_HPP
#define CLIMATEMODEL_VPD_HPP

// Declare the functions
// Function to calculate the saturation vapor pressur
double saturationVaporPressure(double temperature);
// Function to calculate the VPD
double calculateVPD(double temperature, double relativeHumidity);

#endif // CLIMATEMODEL_VPD_HPP
