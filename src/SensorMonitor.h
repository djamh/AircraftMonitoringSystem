//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			Djamal Namoko				//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			  40099428					//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			December 2023				//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SENSOR_MONITOR_H
#define SENSOR_MONITOR_H

#include "FlightDataModels.h"
#include <string>
#include "Utility.h"

class SensorMonitor {
public:
    SensorMonitor();

    void updateReadings(const FlightData& data);
    std::string getTemperatureLampState() const;
    std::string getPressureLampState() const;

    bool isEmergency() const;
    void handleEmergency(FlightData& data);

    void handleSmokeDetection(FlightData& data,bool smokeDetected);
        std::string getSmokeLampState() const;

    void updateFuelLevel(double fuelLevel);
        std::string getFuelLampState() const;


private:
    int tempOutOfRangeCount;
    int pressureOutOfRangeCount;
    std::string temperatureLamp;
    std::string pressureLamp;

    void checkTemperature(double temperature);
    void checkPressure(double pressure);

    bool smokeDetected;
    std::string smokeLamp;

        std::string fuelLamp;

    bool emergencyFlag;

    int emergencyCheckCount;
};

#endif // SENSOR_MONITOR_H
