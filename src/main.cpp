#include "SensorsController.h"
#include <iostream>

int main()
{
    SensorsController& m_SensorsController = SensorsController::getInstance();

    m_SensorsController.initializeSensors();
    m_SensorsController.readAndProcessData();

    std::cout << "Stored sensor values:" << std::endl;
    for (const auto& value : m_SensorsController.getSensorValues())
    {
        std::cout << "Value: " << value.first
                  << ", Timestamp: " << value.second << std::endl;
                  // to be implemented
                  // send it to the MQTT server
    }

    return 0;
}
