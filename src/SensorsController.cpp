#include "SensorsController.h"
#include <iostream>
#include <chrono>

SensorsController::SensorsController()
    : client("tcp://localhost:1883", "sensorsController"),
      callback(std::make_unique<MQTTCallback>())
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

    createSensors();
}

void SensorsController::createSensors()
{
    std::string temperatureSensorId = "Temperature_" + std::to_string(sensorIdCounter++);
    std::string lightSensorId = "Light_" + std::to_string(sensorIdCounter++);
    std::string lightSensorIdSec = "Light_" + std::to_string(sensorIdCounter++);

    temperatureSensor = std::make_unique<TemperatureSensor>(LIGHT_SENSORS_READINGS_LIMIT, temperatureSensorId);
    lightSensor = std::make_unique<LightSensor>(TEMP_SENSORS_READINGS_LIMIT, lightSensorId);
    lightSensorSec = std::make_unique<LightSensor>(TEMP_SENSORS_READINGS_LIMIT, lightSensorIdSec);

    tempSensorThread = std::thread(&TemperatureSensor::run, temperatureSensor.get(), std::ref(client));
    lightSensorThread = std::thread(&LightSensor::run, lightSensor.get(), std::ref(client));
    lightSensorThreadSec = std::thread(&LightSensor::run, lightSensorSec.get(), std::ref(client));

    sensors.push_back(std::move(temperatureSensor));
    sensors.push_back(std::move(lightSensor));
    sensors.push_back(std::move(lightSensorSec));

    tempSensorThread.detach();
    lightSensorThread.detach();
    lightSensorThreadSec.detach();
}

std::string SensorsController::addSensor(std::string sensorType)
{
    if(sensorType == "Temperature")
    {
        std::string newTemperatureSensorId= "Temperature_" + std::to_string(sensorIdCounter++);
        std::unique_ptr<TemperatureSensor> newTemperatureSensor;
        std::thread newTempSensorThread;
        newTemperatureSensor = std::make_unique<TemperatureSensor>(LIGHT_SENSORS_READINGS_LIMIT, newTemperatureSensorId);
        newTempSensorThread = std::thread(&TemperatureSensor::run, newTemperatureSensor.get(), std::ref(client));
        sensors.push_back(std::move(newTemperatureSensor));
        newTempSensorThread.detach();
        return newTemperatureSensorId;
    }
    else if(sensorType == "Light")
    {
        std::string newLightSensorId= "Light_" + std::to_string(sensorIdCounter++);
        std::unique_ptr<TemperatureSensor> newLightSensor;
        std::thread newLightSensorThread;
        newLightSensor = std::make_unique<TemperatureSensor>(LIGHT_SENSORS_READINGS_LIMIT, newLightSensorId);
        newLightSensorThread = std::thread(&TemperatureSensor::run, newLightSensor.get(), std::ref(client));
        sensors.push_back(std::move(newLightSensor));
        newLightSensorThread.detach();
        return newLightSensorId;
    }
    else
    {
        std::cerr << "Error getting specific sensor data: " << sensorType << std::endl;
        return "ERROR";
    }
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
    connOpts.set_keep_alive_interval(60);
    int retries = 5;
    while (retries--)
    {
        std::cout << "remained retries: "  << retries << std::endl;
        try
        {
            client.connect(connOpts)->wait();
            std::cout << "Connected to MQTT server." << std::endl;
            client.subscribe("sensors/#", 1);

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

void SensorsController::storeValue(const std::string& sensorId, double reading)
{
    std::cout << __FUNCTION__ << "is Called in SensorsController." << std::endl;
    std::lock_guard<std::mutex> lock(sensorMutex);
    for (const auto& sensor : sensors)
    {
        if (sensor->getSensorId() == sensorId)
        {
            std::cout << "Sensor ID matched: " << sensor->getSensorId() << std::endl;
            sensor->storeValue(reading);
            return;
        }
    }
    std::cerr << "No matching sensor found for ID: " << sensorId << std::endl;

}

std::string SensorsController::getSpecificSensorData(const std::string& requestedSensorId)
{

    std::cout << __FUNCTION__ << "requested sensor data for ID: " << requestedSensorId << std::endl;
    std::string data;
    for (const auto& sensor : sensors)
    {
        if (sensor->getSensorId() == requestedSensorId)
        {
            std::cout << __FUNCTION__ << sensor->getSensorId() << " sensor data" << std::endl;
            data = sensor->getSpecificSensorData(requestedSensorId);
            break;
        }
    }
    return data;
}

std::vector<std::string> SensorsController::getCreatedSensorsList()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    std::vector<std::string> sensorsList;
    for (const auto& sensor : sensors)
    {
        sensorsList.push_back(sensor->getSensorId());
    }
    return sensorsList;
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

SensorData SensorsController::calculateSensorData(const std::string& sensorId)
{
    std::lock_guard<std::mutex> lock(sensorMutex);
    SensorData m_sensorData;
    for (const auto& sensor : sensors)
    {
        if (sensor->getSensorId() == sensorId)
        {
            m_sensorData = sensor->calculateSensorData();
            break;
        }
    }
    return m_sensorData;
}