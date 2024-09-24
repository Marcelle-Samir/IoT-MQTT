#include "MQTTCallback.h"
#include "SensorsController.h"

void MQTTCallback::storeSensorReading(const std::string& sensorId, double reading)
{
    std::cout << "Storing reading for sensorId: " << sensorId << std::endl;
    SensorsController::getInstance().storeValue(sensorId, reading);
}

void MQTTCallback::message_arrived(mqtt::const_message_ptr msg)
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    std::cout << "Message arrived: " << msg->get_topic() << " -> " << msg->to_string() << std::endl;
    std::string topic = msg->get_topic();
    double reading = std::stod(msg->get_payload_str());
    std::string m_sensorId;
    size_t lastSlashPos = topic.find_last_of('/');
    if (topic == "sensors")
    {
        std::vector<std::string> sensorsList = SensorsController::getInstance().getCreatedSensorsList();
        for (const auto& sensor : sensorsList)
        {
            std::cout << sensor << std::endl;
        }
    }
    else if (topic.find("sensors/") == 0 && topic.find("/data") != std::string::npos)
    {
        size_t start = topic.find("sensors/") + std::string("sensors/").length();
        size_t end = topic.find("/data");
        if (end != std::string::npos)
        {
            m_sensorId = topic.substr(start, end - start);
            double averageReading = SensorsController::getInstance().calculateSensorData(m_sensorId);
            return;
        }
        else
        {
            std::cerr << "Unexpected topic format: " << topic << std::endl;
            return;
        }
    }
    if (lastSlashPos != std::string::npos)
    {
        m_sensorId = topic.substr(lastSlashPos + 1);
    }
    else
    {
        std::cerr << "Unexpected topic format: " << topic << std::endl;
        return;
    }
    storeSensorReading(m_sensorId, reading);
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