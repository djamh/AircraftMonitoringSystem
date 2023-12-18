//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			Djamal Namoko				//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			  40099428					//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			December 2023				//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "FlightDataOutput.h"
#include <iostream>
#include <fstream>

FlightDataOutput::FlightDataOutput() {
    // Constructor implementation
}

FlightDataOutput::~FlightDataOutput() {
    // Destructor implementation
}

void FlightDataOutput::displayAircraftStatus(const AircraftStatus& status) {
    // Example display logic
    std::cout << "Aircraft Status: " << (status.isInCriticalState ? "Critical" : "Normal") << std::endl;
}

void FlightDataOutput::logAlerts(const std::vector<std::string>& alerts) {
    std::ofstream logFile("alerts.log");
    for (const auto& alert : alerts) {
        logFile << alert << std::endl;
        std::cout << "Alert logged: " << alert << std::endl; // Also displaying alert for demonstration
    }
}
