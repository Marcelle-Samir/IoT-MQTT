#include "SensorsController.h"
#include <iostream>
#include <chrono>
#include <memory>

SensorsController::SensorsController()
    : client("tcp://localhost:1883", "sensors_controller"), 
      callback(new MQTTCallback())
{
    client.set_callback(*callback);
    if(connect())
    {
        std::cerr << "ERROR: MQTT connection error" << std::endl;
    }
    else
    {
        std::cout << "Connected to MQTT server." << std::endl;
    }
    createTemperatureSensor();
    createLightSensor();
}

SensorsController::~SensorsController()
{
    delete callback;
    for (Sensor* sensor : sensors)
    {
        delete sensor;
    }
}

void SensorsController::start() {
    std::thread tempThread(&TemperatureSensor::run, temperatureSensor, std::ref(client));
    std::thread lightThread(&LightSensor::run, lightSensor, std::ref(client));
    
    tempThread.detach();
    lightThread.detach();
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

void SensorsController::createTemperatureSensor()
{
    temperatureSensor = new TemperatureSensor();
    sensors.push_back(temperatureSensor);
}

void SensorsController::createLightSensor()
{
    lightSensor = new LightSensor();
    sensors.push_back(lightSensor);
}

void SensorsController::storeValue(const std::string& sensorType, double reading)
{
    std::lock_guard<std::mutex> lock(sensorMutex);
    sensorValues.push_back({sensorType, reading});
    std::cout << "Stored reading from " << sensorType << ": " << reading << std::endl;
}
