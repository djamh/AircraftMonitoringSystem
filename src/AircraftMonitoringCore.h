//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			Djamal Namoko				//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			  40099428					//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			December 2023				//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AIRCRAFT_MONITORING_CORE_H
#define AIRCRAFT_MONITORING_CORE_H

#include "FlightDataModels.h"
#include "SensorMonitor.h"
#include <vector>
#include <string>

class AircraftMonitoringCore {
public:
    AircraftMonitoringCore();
    ~AircraftMonitoringCore();

    void processFlightData(const FlightData& data);
    AircraftStatus determineAircraftStatus(const FlightData& data);
    std::string getTemperatureLampState() const;
    std::string getPressureLampState() const;
    std::string getSmokeLampState() const;
    void updateFuelLevel(double fuelLevel);
    std::string getFuelLampState() const;
    bool isEmergency() const;
    void handleEmergency(FlightData& data);
    void handleSmokeDetection(FlightData& data, bool smokeDetected);

private:
    // Example private members
    SensorMonitor sensorMonitor;
    std::vector<std::string> alerts;
    void generateAlert(const std::string& alertMessage);
};

#endif // AIRCRAFT_MONITORING_CORE_H
