#ifndef SENSORSCONTROLLER_H
#define SENSORSCONTROLLER_H

#include "Sensor.h"
#include "TemperatureSensor.h"
#include "LightSensor.h"
#include <vector>
#include <utility>
#include <string>

class SensorsController
{
public:
    SensorsController(const SensorsController&) = delete;
    SensorsController& operator=(const SensorsController&) = delete;

    static SensorsController& getInstance()
    {
        static SensorsController SensorsControllerInstance;
        return SensorsControllerInstance;
    }

    void initializeSensors();
    void readAndProcessData();
    std::vector<std::pair<float, long long>> getSensorValues() const { return sensorValues; }

private:
    SensorsController();
    ~SensorsController();

    void createTemperatureSensor();
    void createLightSensor();

    void storeValue(float reading, const std::string& sensorType);

    std::vector<Sensor*> sensors;
    std::vector<std::pair<float, long long>> sensorValues;
};

#endif // SENSORSCONTROLLER_H
