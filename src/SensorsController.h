#ifndef SENSORSCONTROLLER_H
#define SENSORSCONTROLLER_H

#include "Sensor.h"
#include <vector>
#include <utility>
#include <string>
#include <memory>
#include <deque>
#include "TemperatureSensor.h"
#include "LightSensor.h"
#include "MQTTCallback.h"

class SensorsController
{
public:
    SensorsController(const SensorsController&) = delete;
    SensorsController& operator=(const SensorsController&) = delete;

    std::deque<std::pair<std::string, double>> getSensorValues() const { return sensorValues; }
    void storeValue(const std::string& sensorType, double reading);
    std::string getSensorData();
    std::string getSpecificSensorData(const std::string& sensorType);

    static SensorsController& getInstance(size_t maxSize)
    {
        static SensorsController instance(maxSize);
        return instance;
    }

private:
    SensorsController(size_t maxSize);
    ~SensorsController();
    int startMosquitto();
    int connect();
    size_t maxSensorValuesSize;
    std::mutex sensorMutex;
    bool running = true;
    std::vector<std::unique_ptr<Sensor>> sensors;
    std::deque<std::pair<std::string, double>> sensorValues;
    std::unique_ptr<TemperatureSensor> temperatureSensor;
    std::unique_ptr<LightSensor> lightSensor;
    std::thread tempSensorThread;
    std::thread lightSensorThread;
    mqtt::async_client client;
    std::unique_ptr<MQTTCallback> callback;
};

#endif // SENSORSCONTROLLER_H
