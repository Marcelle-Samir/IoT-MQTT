#ifndef RESTAPI_H
#define RESTAPI_H

#include <pistache/http.h>
#include <pistache/endpoint.h>
#include "SensorsController.h"
#include <pistache/router.h>
#include <iostream>

class RestApi {
public:
    RestApi(const Pistache::Address& address, SensorsController& controller);
    void start();
    void setupRoutes();
    void getAllSensorData(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void getSpecificSensorData(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void listAllSensors(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);

private:
    SensorsController& sensorsController;
    Pistache::Http::Endpoint httpEndpoint;
    std::mutex sensorMutex;
};

#endif // RESTAPI_H
