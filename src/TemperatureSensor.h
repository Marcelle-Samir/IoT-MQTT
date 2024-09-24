#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "Sensor.h"

class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor(size_t maxReadingSize, const std::string& id)
        : Sensor(maxReadingSize, id), maxSensorValuesSize(maxReadingSize) {}

    ~TemperatureSensor() override;

    double getSensorReading() override;
    double generateReading() override;
    void run(mqtt::async_client& client) override;
    void storeValue(double reading) override;
    std::string generateSensorId() const override { return sensorId; }

private:
    size_t maxSensorValuesSize;

};

#endif // TEMPERATURESENSOR_H
