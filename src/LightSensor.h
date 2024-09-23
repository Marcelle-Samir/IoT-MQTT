#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include "Sensor.h"

class LightSensor : public Sensor
{
public:
    LightSensor();
    ~LightSensor() override;

    double getSensorReading() override;
    double generateReading() override;
    void run(mqtt::async_client& client) override;
};

#endif // LIGHTSENSOR_H
