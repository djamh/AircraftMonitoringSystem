//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			Djamal Namoko				//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			  40099428					//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			December 2023				//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "AircraftMonitoringCore.h"
#include "FlightDataInput.h"
#include "FlightDataOutput.h"
#include "Utility.h"
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <thread>
#include <chrono>
#include <random>
#include <iomanip> // for std::setw and std::setfill


///////////////////// 			IMPORTANT					/////////////////////////////////////////////////////////////////////////////
/* 		      The flow of this program goes as follows :
         * 1. Check planed crashed conditions and distance flown condition
         * 2. Update distance
         * 3. Check for emergencies ( pressure, fuel, temperature, smoke) and display alerts
         * 4. Simulate Pressure changes
         * 5. Simulate Temperature changes
         * 6. Process the data, update the sensors reading
         * 7. Display the data and the sensors/lamp
         */




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 										IMPORTANT					//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////Before Starting, it is recommended, you change some values for easier analysis///////////////////////////////////////////
///////1. isInputAvailable line 64 set the value to 3 for slower flow and easier analysis or 1 to respect the 1 second polling limitation ////////////
///////2. line 106 increase or decrease to modify rate of smoke error happening ///////////////////////////////////////////////////////////////////////
///////3. line 170 increase or decrease to modify rate of pressure error happening////////////////////////////////////////////////////////////////////
///////4. line 194 increase or decrease to modify rate of temperature error happening/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






// MainApplication.cpp, before the simulation loop
std::random_device rd;  // Obtain a random number from hardware
std::mt19937 gen(rd()); // Seed the generator
std::uniform_int_distribution<> distr(0, 100); // Define the range for increments
std::uniform_real_distribution<> distrChance(0.0, 1.0); // For random chance (0% to 100%)
// Function to display sensor dials


bool isInputAvailable1() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 3;  // wait for 3 second, easier for testing, put 1 instead to respect the 1 second polling limitation
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

void displayDials(const FlightData& data) {
	std::cout << "Temperature Dial: " << data.temperature << " °C ";
	std::cout << "Pressure Dial: " << data.pressure << " Pa " << std::endl;
}


////This is the main loop right here
int main() {
    try {

    	//Initializing values for loop start

        AircraftMonitoringCore monitoringCore;
        FlightDataOutput dataOutput;
        FlightData data;
        data.temperature = 1300.0; // Starting temperature
        data.pressure = 500.0;     // Starting pressure
        data.fuelLevel = 100;		//Starting fuel level percentage
        data.altitude = 0;
        // Simulation parameters
        const int maxAltitude = 50000;
        const int timeInterval = 1000; // Time in milliseconds
        double distanceFlown = 0;
        const double maxDistance = 5000.0; // Maximum distance to be flown in km
        const double descentStartDistance = 0.9*maxDistance;
        bool planeCrashed = false;
        bool displayrates = false;

        // Random number generation setup
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrIncrementTemp(0, 50); // For random increments temp
        std::uniform_int_distribution<> distrIncrementPress(0, 50); // For random increments pressure
        std::uniform_real_distribution<> distrDirection(-1.0, 1.0);// For deciding temperature direction
        std::uniform_real_distribution<> distrChance(0.0, 1.0); // For random chance (0% to 100%)
        const double smokeDetectionChance = 0.05; //Chance of Smoke


        // while loop and main loop
        while (!planeCrashed && distanceFlown < maxDistance) {

        	// Loop for distance a safe flight status and altitude

        	if (distanceFlown <= descentStartDistance && data.altitude < maxAltitude) {
        	            data.altitude += 5000;
        	            if (data.altitude > maxAltitude) {
        	                data.altitude = maxAltitude; // Ensure altitude does not exceed max
        	            }
        	        }
        	        distanceFlown += 125 + (std::rand() % (200 - 125 + 1));

        	        // Start decreasing altitude when distance flown is greater than descentStartDistance
        	                if (distanceFlown > descentStartDistance && data.altitude > 0) {
        	                    double altitudeDecrement = data.altitude / (maxDistance - distanceFlown);
        	                    data.altitude -= altitudeDecrement;
        	                    if (data.altitude < 0) {
        	                        data.altitude = 0; // Ensure altitude does not go below 0
        	                    }
        	                }


        // Check crash conditions
        	        if (data.temperature > 3000 || data.pressure > 1500 || data.fuelLevel <= 0) {
        	            planeCrashed = true;
        	            std::cout << "Plane crashed!" << std::endl;
        	            break;
        	        }

        	//////////////////////////////////////////////////////Emergency check ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check for emergencies and handle them
        	            if (monitoringCore.isEmergency()) {
        	                monitoringCore.handleEmergency(data);
        	            }

        	 // Generate a random chance
        	    double smokechance = distrChance(gen);

        	 // Determine smoke detection based on chance
        	    if (!data.smokeDetected){
        	    data.smokeDetected = (smokechance < smokeDetectionChance);
        	    }

        	 // Handle smoke detection
        	    monitoringCore.handleSmokeDetection(data, data.smokeDetected);

        	 // Update and check fuel level
        	            data.fuelLevel = data.fuelLevel-1;
        	            monitoringCore.updateFuelLevel(data.fuelLevel);




//////////////////////////////////////////////////////Pressure simulation ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        	// Pressure variation logic
        	            int pressIncrement = distrIncrementPress(gen);
        	            double pressChance = distrChance(gen); // Generate a random chance for pressure

        	            // Apply 5x increment based on chance for pressure
        	            const double pressPercentChance = 0.1; // 10% chance, for example
        	            if (pressChance < pressPercentChance) {
        	                pressIncrement *= 20; // Increase increment by 5 times for pressure
        	            }

        	            // Update pressure with fluctuation
        	            if (data.pressure < 900) {
        	                data.pressure += pressIncrement;
        	            } else {
        	            	if(data.pressure < 1100){
        	                // Random fluctuation up or down for pressure
        	                data.pressure += pressIncrement * distrDirection(gen);//add + 100 here to be sure to reach an alert
        	            	} else {
        	            		data.fuelLevel = data.fuelLevel-1; //reduce fuel again when exceeding temperature range
        	            	}
        	            }


//////////////////////////////////////////////////////Temperature simulation ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        	// Determine temperature increment
        	            int tempIncrement = distrIncrementTemp(gen);
        	            double chance = distrChance(gen); // Generate a random chance

        	            // Apply 5x increment based on chance
        	            const double xPercentChance = 0.2; // 20% chance, for example. Replace with your x/100 value
        	            if (chance < xPercentChance) {
        	                tempIncrement *= 10; // Increase increment by 5 times
        	            }

        	            // Update temperature with fluctuation
        	            if (data.temperature < 1900) {
        	                data.temperature += tempIncrement;
        	            } else {
        	            	if(data.temperature < 2100){
        	                // Random fluctuation up or down
        	                data.temperature += tempIncrement * distrDirection(gen);//add + 100 here to be sure to reach an alert
        	            	} else {
        	            		data.fuelLevel = data.fuelLevel-1; //reduce fuel again when exceeding temperature range
        	            	}
        	            }



//////////////////////////////////////////////////////Processing //////////////////////////////////////////////////////////////////////////////////////////////////////
            // Process flight data
            monitoringCore.processFlightData(data);

            data.rateOfPressureChange = data.pressure - data.lastPressure;
            data.rateOfFuelConsumption = data.lastFuelLevel - data.fuelLevel;
            data.rateOfTemperatureChange = data.temperature - data.lastTemperature;
            data.lastPressure = data.pressure;
            data.lastFuelLevel = data.fuelLevel;
            data.lastTemperature = data.temperature;


//////////////////////////////////////////////////////Display //////////////////////////////////////////////////////////////////////////////////////////////////////
            // Display sensor dials
            std::cout << std::setfill('=') << std::setw(60) << "=" << std::endl;
               std::cout << " Flight AC874 Status " << "    " << "Distance to Destination : " << maxDistance-distanceFlown <<" Km" <<std::endl;
               std::cout << std::setfill('=') << std::setw(60) << "=" << std::endl;

            displayDials(data);
            std::cout << "Temperature Lamp: " << monitoringCore.getTemperatureLampState() << ", ";
            std::cout << "Pressure Lamp: " << monitoringCore.getPressureLampState() << std::endl;
            std::cout << "Smoke Lamp: " << monitoringCore.getSmokeLampState() << std::endl;

            // Display current altitude,fuel level and status
            AircraftStatus status = monitoringCore.determineAircraftStatus(data);
            std::cout << "Current Altitude: " << data.altitude << " feet, ";
            std::cout << "Status: " << (status.isInCriticalState ? "Critical" : "Normal") << std::endl;
            std::cout << "Fuel Level: " << data.fuelLevel << "%, Fuel Lamp: " << monitoringCore.getFuelLampState() << std::endl;


            // Additional Options
                        std::cout << std::setfill('=') << std::setw(60) << "=" << std::endl;


                        if(displayrates){
                                             	   std::cout << "Rate of Pressure Change: " << data.rateOfPressureChange << " Pa/sec" << std::endl;
                                             	   std::cout << "Rate of Fuel Consumption: " << data.rateOfFuelConsumption << " %/sec" << std::endl;
                                             	   std::cout << "Rate of Temperature Change: " << data.rateOfTemperatureChange << " °C/sec" << std::endl;
                                             	   std::cout << std::setfill('=') << std::setw(60) << "=" << std::endl;
                                                }

                        std::cout << "Press '1' for additional options" << std::endl;

                        if (isInputAvailable1()) {
                               char userInput;
                               std::cin >> userInput;

                               if (userInput == '1') {
                                   // Display additional options
                                   std::cout << "Press 1 to simulate a smoke signal on next loop" << std::endl;
                                   if(!displayrates){
                                   std::cout << "Press 2 to get additional data" << std::endl;
                                   }
                                   std::cin >> userInput; // Wait for user to choose an option

                                   if (userInput == '1') {
                                       data.smokeDetected = true;
                                   } else if (userInput == '2') {
                                	   displayrates = true ;
                                   }

                                   // After handling the option, break out of the loop or continue based on your program logic
                                   std::cout << std::endl;
                                  continue;
                               }
                           }

           // Add a new line for spacing between each set of outputs
            std::cout << std::setfill('=') << std::setw(60) << "=" << std::endl;
                       std::cout << std::endl;


            //std::this_thread::sleep_for(std::chrono::milliseconds(timeInterval));
        }
        // Check if the destination was reached
            if (!planeCrashed && distanceFlown >= maxDistance) {
                std::cout << "You have arrived!" << std::endl;
            }

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


