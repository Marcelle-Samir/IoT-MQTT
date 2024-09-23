#include "MQTTCallback.h"
#include "SensorsController.h"

void MQTTCallback::storeSensorReading(const std::string& sensorType, double reading)
{
    SensorsController::getInstance(20).storeValue(sensorType, reading);
}

void MQTTCallback::message_arrived(mqtt::const_message_ptr msg)
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    std::cout << "Message arrived: " << msg->get_topic() << " -> " << msg->to_string() << std::endl;
    std::string topic = msg->get_topic();
    double reading = std::stod(msg->get_payload_str());
    std::string sensorType;
    if (topic == "sensors/temperature")
    {
        sensorType = "TemperatureSensor";
    }
    else if (topic == "sensors/light")
    {
        sensorType = "LightSensor";
    }
    storeSensorReading(sensorType, reading);
}

void MQTTCallback::on_failure(const mqtt::token& tok)
{
    std::cout << "Connection failed." << std::endl;
}

void MQTTCallback::on_success(const mqtt::token& tok)
{
    std::cout << "Connection successful." << std::endl;
}

void MQTTCallback::connected(const std::string& cause)
{
    std::cout << "Connected: " << cause << std::endl;
}

void MQTTCallback::connection_lost(const std::string& cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}

void MQTTCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    std::cout << "Delivery complete." << std::endl;
}