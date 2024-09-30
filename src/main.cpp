#include <iostream>
#include <thread>
#include "SensorsController.h"
#include "RestApi.h"
#include "gRPCServer.h"

int main()
{
    std::cout << "Main is Called." << std::endl;

    SensorsController& m_SensorsController = SensorsController::getInstance();

    auto port = 9080;
    RestApi restApi(Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(port)), m_SensorsController);
    std::thread restApiThread([&restApi]() {
        restApi.start();
    });
    restApiThread.detach();

    gRPCServer grpcServer(m_SensorsController, std::string("172.17.0.2:50051"));
    std::thread grpcThread([&grpcServer]() {
        grpcServer.Run();
    });
    grpcThread.detach();

    std::this_thread::sleep_for(std::chrono::hours(1));

    return 0;
}
