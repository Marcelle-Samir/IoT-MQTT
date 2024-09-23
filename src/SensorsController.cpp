#include "SensorsController.h"
#include <iostream>
#include <chrono>

SensorsController::SensorsController()
    : client("tcp://localhost:1883", "sensorsController"), 
      callback(std::make_unique<MQTTCallback>()),
      temperatureSensor(std::make_unique<TemperatureSensor>()),
      lightSensor(std::make_unique<LightSensor>())
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    if(startMosquitto())
    {
        std::cerr << "ERROR: startMosquitto failed" << std::endl;
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

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

    tempSensorThread = std::thread(&TemperatureSensor::run, temperatureSensor.get(), std::ref(client));
    lightSensorThread = std::thread(&LightSensor::run, lightSensor.get(), std::ref(client));

    sensors.push_back(std::move(temperatureSensor));
    sensors.push_back(std::move(lightSensor));
}

SensorsController::~SensorsController()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;

    if (tempSensorThread.joinable())
    {
        tempSensorThread.join();
    }
    if (lightSensorThread.joinable())
    {
        lightSensorThread.join();
    }
}

int SensorsController::connect()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);
    connOpts.set_keep_alive_interval(20);
    int retries = 5;
    while (retries--)
    {
        std::cout << "remained retries: "  << retries << std::endl;
        try
        {
            client.connect(connOpts)->wait();
            std::cout << "Connected to MQTT server." << std::endl;
            client.subscribe("sensors/temperature", 1);
            client.subscribe("sensors/light", 1);
            return 0;
        }
        catch (const mqtt::exception& e)
        {
            std::cerr << "MQTT connection error: " << e.what() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
    return 1;
}

void SensorsController::storeValue(const std::string& sensorType, double reading)
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    std::lock_guard<std::mutex> lock(sensorMutex);
    sensorValues.push_back({sensorType, reading});
    std::cout << "Stored reading from " << sensorType << ": " << reading << std::endl;
}

int SensorsController::startMosquitto()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    int result = system("mosquitto -c /etc/mosquitto/mosquitto.conf &");
    if (result == -1)
    {
        std::cerr << "Failed to start Mosquitto." << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Mosquitto started." << std::endl;
        return 0;
    }
}