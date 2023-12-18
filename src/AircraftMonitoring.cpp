//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			Djamal Namoko				//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			  40099428					//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			December 2023				//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "AircraftMonitoringCore.h"
#include <iostream>

AircraftMonitoringCore::AircraftMonitoringCore() {
    // Constructor implementation (if needed)
}

AircraftMonitoringCore::~AircraftMonitoringCore() {
    // Destructor implementation (if needed)
}

void AircraftMonitoringCore::processFlightData(const FlightData& data) {
    // Example processing logic
    AircraftStatus status = determineAircraftStatus(data);

    if (status.isInCriticalState) {
        generateAlert("Aircraft in critical state!");
    }

    // Update sensor readings
       sensorMonitor.updateReadings(data);
}

AircraftStatus AircraftMonitoringCore::determineAircraftStatus(const FlightData& data) {
    // Simplified logic for determining aircraft status
    AircraftStatus status;
    status.isInCriticalState = (data.altitude < 000); // Example condition
    // Check for other emergencies
        if (sensorMonitor.isEmergency() || sensorMonitor.getTemperatureLampState() == "Red" ||
            sensorMonitor.getPressureLampState() == "Red" || sensorMonitor.getSmokeLampState() == "Red") {
            status.isInCriticalState = true;
        }
    return status;
}

void AircraftMonitoringCore::generateAlert(const std::string& alertMessage) {
    alerts.push_back(alertMessage);
    std::cout << "Alert: " << alertMessage << std::endl; // Displaying alert for demonstration
}

std::string AircraftMonitoringCore::getTemperatureLampState() const {
    return sensorMonitor.getTemperatureLampState();
}

std::string AircraftMonitoringCore::getPressureLampState() const {
    return sensorMonitor.getPressureLampState();
}

bool AircraftMonitoringCore::isEmergency() const {
    return sensorMonitor.isEmergency();
}

void AircraftMonitoringCore::handleEmergency(FlightData& data) {
    sensorMonitor.handleEmergency(data);
}

void AircraftMonitoringCore::handleSmokeDetection(FlightData& data, bool smokeDetected) {
    sensorMonitor.handleSmokeDetection(data, smokeDetected);
}

std::string AircraftMonitoringCore::getSmokeLampState() const {
    return sensorMonitor.getSmokeLampState();
}

void AircraftMonitoringCore::updateFuelLevel(double fuelLevel) {
    sensorMonitor.updateFuelLevel(fuelLevel);
}

std::string AircraftMonitoringCore::getFuelLampState() const {
    return sensorMonitor.getFuelLampState();
}
