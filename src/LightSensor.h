#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include "Sensor.h"

class LightSensor : public Sensor
{
public:
    LightSensor(size_t maxReadingSize, const std::string& id)
        : Sensor(maxReadingSize, id), maxSensorValuesSize(maxReadingSize) {}
    ~LightSensor() override;

    double getSensorReading() override;
    double generateReading() override;
    void run(mqtt::async_client& client) override;
    void storeValue(double reading) override;
    std::string generateSensorId() const override { return sensorId; }

private:
    size_t maxSensorValuesSize;

};

#endif // LIGHTSENSOR_H
