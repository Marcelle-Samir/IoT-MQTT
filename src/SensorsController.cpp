#include "SensorsController.h"
#include <iostream>
#include <chrono>

SensorsController::SensorsController()
    : client("tcp://localhost:1883", "sensorsController"), 
      callback(std::make_unique<MQTTCallback>()),
      temperatureSensor(std::make_unique<TemperatureSensor>()),
      lightSensor(std::make_unique<LightSensor>())
{
    client.set_callback(*callback);
    if(connect())
    {
        std::cerr << "ERROR: MQTT connection error" << std::endl;
        std::cout << "Attempting to connect to MQTT broker at " << client.get_server_uri() << std::endl;
    }
    else
    {
        std::cout << "Connected to MQTT server." << std::endl;
    }
    sensors.push_back(std::move(temperatureSensor));
    sensors.push_back(std::move(lightSensor));
}

SensorsController::~SensorsController()
{
}

int SensorsController::connect()
{
    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);
    connOpts.set_keep_alive_interval(20);
    try
    {
        client.connect(connOpts)->wait();
        std::cout << "Connected to MQTT server." << std::endl;
        client.subscribe("sensors/temperature", 1);
        client.subscribe("sensors/light", 1);
    }
    catch (const mqtt::exception& e)
    {
        std::cerr << "MQTT connection error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

void SensorsController::storeValue(const std::string& sensorType, double reading)
{
    std::lock_guard<std::mutex> lock(sensorMutex);
    sensorValues.push_back({sensorType, reading});
    std::cout << "Stored reading from " << sensorType << ": " << reading << std::endl;
}
