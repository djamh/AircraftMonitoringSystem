//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			Djamal Namoko				//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			  40099428					//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			December 2023				//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "FlightDataInput.h"
#include <fstream>
#include <iostream>
using namespace std;
#include <sstream>

FlightDataInput::FlightDataInput() {
    // Constructor implementation
}

FlightDataInput::~FlightDataInput() {
    // Destructor implementation
}

std::vector<FlightData> FlightDataInput::readFlightData(const std::string& dataSource) {
    std::vector<FlightData> flightDataList;
    std::string line;
    int numOfEntries;

    std::cout << "Enter the number of flight data entries: ";
    std::cin >> numOfEntries;
    std::cin.ignore(); // To ignore the newline after the number input

    std::cout << "Enter flight data in the format: altitude speed\n";
    for (int i = 0; i < numOfEntries; ++i) {
        std::getline(std::cin, line);
        flightDataList.push_back(parseDataLine(line));
    }

    return flightDataList;
}

FlightData FlightDataInput::parseDataLine(const std::string& dataLine) {
    std::istringstream iss(dataLine);
    FlightData data;
    iss >> data.altitude >> data.speed;
    // Parse other data fields as necessary
    return data;
}
