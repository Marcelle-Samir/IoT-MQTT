#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include "Sensor.h"

class LightSensor : public Sensor
{
public:
    LightSensor();
    ~LightSensor() override;

    void reportSensorReading(float reading) override;
    std::pair<float, long long> getSensorReading() override;
};

#endif // LIGHTSENSOR_H
