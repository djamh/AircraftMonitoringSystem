//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			Djamal Namoko				//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			  40099428					//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			December 2023				//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SensorMonitor.h"
#include <iostream>
#include <sys/select.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include "Utility.h"

bool isInputAvailable() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 3; // Set timeout to 3 seconds
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

SensorMonitor::SensorMonitor()
    : tempOutOfRangeCount(0), pressureOutOfRangeCount(0),
      temperatureLamp("Green"), pressureLamp("Green"),
	  emergencyFlag(false){}

void SensorMonitor::updateReadings(const FlightData& data) {
    checkTemperature(data.temperature);
    checkPressure(data.pressure);
}

std::string SensorMonitor::getTemperatureLampState() const {
    return temperatureLamp;
}

std::string SensorMonitor::getPressureLampState() const {
    return pressureLamp;
}

void SensorMonitor::checkTemperature(double temperature) {
    if (temperature < 1000.0 || temperature > 2000.0) {
        tempOutOfRangeCount++;
        if (tempOutOfRangeCount >= 3) {
            temperatureLamp = "Red";
            emergencyFlag = true;
            Utility::logAlert("Temperature");
        }
    } else {
        tempOutOfRangeCount = 0;
    }
}

void SensorMonitor::checkPressure(double pressure) {
    if (pressure < 300.0 || pressure > 1000.0) {
        pressureOutOfRangeCount++;
        if (pressureOutOfRangeCount >= 3) {
            pressureLamp = "Red";
            emergencyFlag = true;
            Utility::logAlert("Pressure");
        }
    } else {
        pressureOutOfRangeCount = 0;
    }
}

bool SensorMonitor::isEmergency() const {
    return emergencyFlag;
}

void SensorMonitor::handleEmergency(FlightData& data) {
std::cout << "Critical Alert" << std::endl << "Enter 'x' to handle the emergency: "<< std::endl;
std::cout.flush();

if (isInputAvailable()) {
        char userInput;
        read(STDIN_FILENO, &userInput, 1); // Read a single character

if (userInput == 'x' || userInput == 'X') {
    if (temperatureLamp == "Red") {
        std::cout << "The Temperature is too high, Reduce it by inputting a value: ";
        double reduction;
        std::cin >> reduction;
        data.temperature -= reduction;
        if (data.temperature >= 1000.0 && data.temperature <= 2000.0) {
            temperatureLamp = "Green";
            tempOutOfRangeCount = 0;
            std::cout << "Temperature successfully reduced! " << std::endl;
            emergencyFlag = false;
        }
    }
    if (pressureLamp == "Red") {
    	 std::cout << "The Pressure is too high, Reduce it by inputting a value: ";
    	        double reduction;
    	        std::cin >> reduction;
    	        data.pressure -= reduction;
    	        if (data.pressure >= 300.0 && data.pressure <= 1000.0) {
    	            pressureLamp = "Green";
    	            pressureOutOfRangeCount = 0;
    	            std::cout << "Pressure successfully reduced! " << std::endl;
    	            emergencyFlag = false;
    	        }
    }
    if (smokeLamp == "Red") {
    	        std::cout << "Smoke detected! Input any values to activate fire extinguishers ! " << std::endl;
    	        char userInput2;
    	        std::cin >> userInput2;
    	         smokeLamp = "Green";
    	         std::cout << "Fire extinguishers activated! The smoke disappeared ! " << std::endl;
    	         emergencyFlag = false;
    	         data.smokeDetected = false;
        	      }


 if (fuelLamp == "Red") {
	        std::cout << "Fuel Levels Critical! You will crash at 0, press anything to refuel your engine ! " << std::endl;
	        char userInput;
	        std::cin >> userInput;
	         // Increase the fuel level by 10 and turn the fuel lamp green
	            data.fuelLevel += 10;
	            fuelLamp = "Green";
	            std::cout << "Engine refueled!" <<  std::endl;
	            emergencyFlag = false;
    	      }

}



}
else {
        std::cout << "No input detected within 2 seconds, continuing..." << std::endl;
    }
std::cout << std::endl;
}


void SensorMonitor::handleSmokeDetection(FlightData& data,bool smokeDetected) {
    data.smokeDetected = smokeDetected;
    smokeLamp = data.smokeDetected ? "Red" : "Green";
    if (data.smokeDetected) {
            emergencyFlag = true; // Set emergency flag if smoke is detected
            Utility::logAlert("Smoke");

        }
}

std::string SensorMonitor::getSmokeLampState() const {
    return smokeLamp;
}


void SensorMonitor::updateFuelLevel(double fuelLevel) {
    // Update the fuel level reading
    if (fuelLevel <= 10.0) { // Fuel level is under 10%
        fuelLamp = "Red";    // Set the fuel lamp to red
        emergencyFlag = true; // Set the emergency flag to true
        Utility::logAlert("Fuel");
    } else {
        fuelLamp = "Green";   // Set the fuel lamp to green
    }
}

std::string SensorMonitor::getFuelLampState() const {
    return fuelLamp;
}
