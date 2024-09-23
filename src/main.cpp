#include "SensorsController.h"
#include <iostream>

int main()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;

    SensorsController& m_SensorsController = SensorsController::getInstance();
    
    std::this_thread::sleep_for(std::chrono::hours(1));

    return 0;
}
