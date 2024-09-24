#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "Sensor.h"

class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor(size_t maxReadingSize, const std::string& id)
        : Sensor(maxReadingSize, id), maxSensorValuesSize(maxReadingSize), sensorValues() {}

    ~TemperatureSensor() override;

    double getSensorReading() override;
    double generateReading() override;
    void run(mqtt::async_client& client) override;
    void storeValue(double reading) override;
    std::string getSensorData() override;
    std::string getSpecificSensorData(const std::string& requestedSensorId) override;
    std::deque<std::pair<std::string, double>> getSensorValues() const { return sensorValues; }
    std::string generateSensorId() const override { return sensorId; }
    double calculateSensorData() override;

private:
    size_t maxSensorValuesSize;
    std::mutex sensorMutex;
    std::deque<std::pair<std::string, double>> sensorValues;
};

#endif // TEMPERATURESENSOR_H
