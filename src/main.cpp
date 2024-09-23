#include <iostream>
#include "SensorsController.h"
#include "RestApi.h"

int main()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;

    SensorsController& m_SensorsController = SensorsController::getInstance();

    auto port = 9080;
    RestApi restApi(Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(port)), m_SensorsController);
    std::thread restApiThread([&restApi]() {
    restApi.start();
    });

    std::this_thread::sleep_for(std::chrono::hours(1));

    return 0;
}
