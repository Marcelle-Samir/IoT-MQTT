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
        static SensorsController instance;
        return instance;
    }

    std::vector<std::pair<std::string, double>> getSensorValues() const { return sensorValues; }
    void storeValue(const std::string& sensorType, double reading);

private:
    SensorsController();
    ~SensorsController();
    int startMosquitto();
    int connect();
    std::mutex sensorMutex;
    bool running = true;
    std::vector<std::unique_ptr<Sensor>> sensors;
    std::vector<std::pair<std::string, double>> sensorValues;
    std::unique_ptr<TemperatureSensor> temperatureSensor;
    std::unique_ptr<LightSensor> lightSensor;
    std::thread tempSensorThread;
    std::thread lightSensorThread;
    mqtt::async_client client;
    std::unique_ptr<MQTTCallback> callback;
};

#endif // SENSORSCONTROLLER_H
