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
#include "SensorData.h"

#define LIGHT_SENSORS_READINGS_LIMIT    15
#define TEMP_SENSORS_READINGS_LIMIT     20

class SensorsController
{
public:
    SensorsController(const SensorsController&) = delete;
    SensorsController& operator=(const SensorsController&) = delete;

    std::deque<std::pair<std::string, double>> getSensorValues() const { return sensorValues; }
    void storeValue(const std::string& sensorId, double reading);
    std::string getSpecificSensorData(const std::string& requestedSensorId);
    std::vector<std::string> getCreatedSensorsList();
    SensorData calculateSensorData(const std::string& sensorId);
    static SensorsController& getInstance()
    {
        static SensorsController instance;
        return instance;
    }
    std::string addSensor(std::string sensorType);
private:
    SensorsController();
    ~SensorsController();
    int startMosquitto();
    int connect();
    void createSensors();
    std::mutex sensorMutex;
    bool running = true;
    std::vector<std::unique_ptr<Sensor>> sensors;
    std::deque<std::pair<std::string, double>> sensorValues;
    std::unique_ptr<TemperatureSensor> temperatureSensor;
    std::unique_ptr<LightSensor> lightSensor;
    std::unique_ptr<LightSensor> lightSensorSec;
    std::thread tempSensorThread;
    std::thread lightSensorThread;
    std::thread lightSensorThreadSec;
    mqtt::async_client client;
    std::unique_ptr<MQTTCallback> callback;
    int sensorIdCounter = 0;
};

#endif // SENSORSCONTROLLER_H
