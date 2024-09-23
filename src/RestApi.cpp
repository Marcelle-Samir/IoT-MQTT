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

    httpEndpoint.setHandler(router.handler());
}

void RestApi::getAllSensorData(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
    auto sensorType = request.param(":sensorType").as<std::string>();
    try
    {
        std::string data = sensorsController.getSensorData(sensorType);
        response.send(Pistache::Http::Code::Ok, data);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error getting sensor data: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error, "Error retrieving data");
    }
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

void RestApi::listAllSensors(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response) {
    std::vector<std::string> sensorsList = SensorsController::getInstance().getCreatedSensorsList();
    std::ostringstream sensorData;
    for (const auto& sensor : sensorsList)
    {
        std::cout << sensor << std::endl;
        sensorData << sensor << "\n";
    }
    response.send(Pistache::Http::Code::Ok, sensorData.str());
}