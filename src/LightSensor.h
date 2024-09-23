#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include "Sensor.h"

class LightSensor : public Sensor
{
public:
    LightSensor(size_t maxReadingSize, const std::string& id)
        : Sensor(maxReadingSize, id), maxSensorValuesSize(maxReadingSize), sensorValues() {}
    ~LightSensor() override;

    double getSensorReading() override;
    double generateReading() override;
    void run(mqtt::async_client& client) override;
    void storeValue(double reading) override;
    std::string getSensorData() override;
    std::string getSpecificSensorData(const std::string& requestedSensorId) override;
    std::deque<std::pair<std::string, double>> getSensorValues() const { return sensorValues; }
    std::string generateSensorId() const override { return sensorId; }

private:
    size_t maxSensorValuesSize;
    std::mutex sensorMutex;
    std::deque<std::pair<std::string, double>> sensorValues;

};

#endif // LIGHTSENSOR_H
