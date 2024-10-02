#include <iostream>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "sensor.grpc.pb.h"
#include "SensorsController.h"
#include "RestApi.h"
#include "gRPCClient.h"
#include "gRPCServer.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

ABSL_FLAG(std::string, target, "localhost:50051", "gRPC server address");

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

    gRPCServer grpcServer(m_SensorsController, "localhost:50051");
    std::thread grpcThread([&grpcServer]() {
        grpcServer.Run();
    });
    grpcThread.detach();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::string target_str = absl::GetFlag(FLAGS_target);
    gRPCClient client(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

    std::string response = client.GetSensorsList("dummy");
    std::cout << "Server Response: " << response << std::endl;

    std::this_thread::sleep_for(std::chrono::hours(1));

    return 0;
}
