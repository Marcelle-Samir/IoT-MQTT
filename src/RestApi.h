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
    void getSensorData(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

private:
    SensorsController& sensorsController;
    Pistache::Http::Endpoint httpEndpoint;
};

#endif // RESTAPI_H
