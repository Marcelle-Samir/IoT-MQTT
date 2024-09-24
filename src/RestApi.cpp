#include "RestApi.h"

RestApi::RestApi(const Pistache::Address& address, SensorsController& controller)
    : sensorsController(controller), httpEndpoint(address)
{
}

void RestApi::start()
{
    setupRoutes();
    httpEndpoint.init();
    std::cout << "HTTP server is starting..." << std::endl;
    httpEndpoint.serve();
    std::cout << "HTTP server has stopped." << std::endl;
}

void RestApi::setupRoutes()
{
    using namespace Pistache;
    using namespace Pistache::Rest;

    Rest::Router router;
    Routes::Get(router, "/sensors", Routes::bind(&RestApi::listAllSensors, this));
    Routes::Get(router, "/sensors/:sensorType", Routes::bind(&RestApi::getSpecificSensorData, this));
    Routes::Get(router, "/sensors/:sensorType/data", Routes::bind(&RestApi::calculateSensorData, this));

    httpEndpoint.setHandler(router.handler());
}

void RestApi::getSpecificSensorData(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    try
    {
        auto sensorType = request.param(":sensorType").as<std::string>();
        std::string data = sensorsController.getSpecificSensorData(sensorType);

        if (data.empty())
        {
            response.send(Pistache::Http::Code::Not_Found, "Sensor data not found");
        }
        else
        {
            response.send(Pistache::Http::Code::Ok, data);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error getting specific sensor data: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error, "Error retrieving data");
    }
}

void RestApi::listAllSensors(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response)
{
    std::vector<std::string> sensorsList = SensorsController::getInstance().getCreatedSensorsList();
    std::ostringstream sensorData;
    for (const auto& sensor : sensorsList)
    {
        std::cout << sensor << std::endl;
        sensorData << sensor << "\n";
    }
    response.send(Pistache::Http::Code::Ok, sensorData.str());
}

void RestApi::calculateSensorData(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    try
    {
        auto sensorType = request.param(":sensorType").as<std::string>();
        SensorData data = sensorsController.calculateSensorData(sensorType);

        std::string jsonResponse = "{\"averageReading\": " + std::to_string(data.averageReading) +
                                   ", \"minReading\": " + std::to_string(data.minReading) +
                                   ", \"maxReading\": " + std::to_string(data.maxReading) + "}\n";

        if (data.averageReading == 0.0 && data.minReading == std::numeric_limits<double>::max() &&
            data.maxReading == std::numeric_limits<double>::min())
        {
            response.send(Pistache::Http::Code::Not_Found, "Sensor data not found");
        }
        else
        {
            response.send(Pistache::Http::Code::Ok, jsonResponse);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error getting specific sensor data: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error, "Error retrieving data");
    }
}
