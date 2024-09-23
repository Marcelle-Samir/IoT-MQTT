#include "Sensor.h"
#include <iostream>

Sensor::Sensor()
{
}

Sensor::~Sensor()
{
}

void Sensor::displayData()
{
    std::cout << "Sensor value: " << sensorReading << std::endl;
    std::cout << "Reading time: " << readingTime << std::endl;
}

void Sensor::publishReading(mqtt::async_client& client, const std::string& topic, double value)
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    mqtt::message_ptr pubmsg = mqtt::make_message(topic, std::to_string(value));
    pubmsg->set_qos(1);
    client.publish(pubmsg);
}