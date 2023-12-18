//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			Djamal Namoko				//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			  40099428					//////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 			December 2023				//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Utility.h"
#include <iomanip>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iostream>

namespace Utility {

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string getCurrentTime();
        // Add more utility functions as needed

    void logAlert(const std::string& alertType) {
        // Hardcoded file path and name
        std::string filename = "/alerts.txt";

        // Open the file in append mode
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
           // std::cerr << "Error opening file for writing." << std::endl;
            return;
        }

        // Get the current time
        std::time_t now = std::time(nullptr);
        char* dt = std::ctime(&now);

        // Write the alert and time to the file
        file << alertType << " alert at " << dt; // ctime() adds a newline

        // Close the file
        file.close();
    }

    // Implementation of other utility functions

}
