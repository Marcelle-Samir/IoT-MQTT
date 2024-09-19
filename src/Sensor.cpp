#include "Sensor.h"
#include <iostream>

Sensor::Sensor()
{
}

Sensor::~Sensor()
{
}

void Sensor::displayData()
{
    std::cout << "Sensor value: " << sensorReading << std::endl;
    std::cout << "Reading time: " << readingTime << std::endl;
}

