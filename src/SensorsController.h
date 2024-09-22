#ifndef SENSORSCONTROLLER_H
#define SENSORSCONTROLLER_H

#include "Sensor.h"
#include <vector>
#include <utility>
#include <string>
#include <memory>
#include "TemperatureSensor.h"
#include "LightSensor.h"
#include "MQTTCallback.h"

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

    std::vector<std::pair<std::string, double>> getSensorValues() const { return sensorValues; }
    void storeValue(const std::string& sensorType, double reading);

private:
    SensorsController();
    ~SensorsController();
    int connect();
    void start();
    void createTemperatureSensor();
    void createLightSensor();
    std::mutex sensorMutex;

    std::vector<Sensor*> sensors;
    std::vector<std::pair<std::string, double>> sensorValues;
    TemperatureSensor* temperatureSensor = new TemperatureSensor();
    LightSensor* lightSensor = new LightSensor();
    mqtt::async_client client;
    mqtt::callback* callback;
};

#endif // SENSORSCONTROLLER_H
