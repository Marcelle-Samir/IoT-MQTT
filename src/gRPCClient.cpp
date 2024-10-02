#include "gRPCClient.h"
#include <iostream>
#include <sstream>

gRPCClient::gRPCClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(sensor::SensorService::NewStub(channel)) {}

std::string gRPCClient::GetSensorsList(const std::string& sensor_type) {
    sensor::SensorRequest request;
    request.set_sensor_type(sensor_type);

    sensor::SensorList response;
    grpc::ClientContext context;

    grpc::Status status = stub_->GetSensorsList(&context, request, &response);

    if (!status.ok()) {
        std::cerr << "RPC failed: " << status.error_message() << std::endl;
        return "RPC failed";
    }

    std::ostringstream sensorData;
    for (const auto& sensor : response.sensors()) {
        sensorData << sensor << "\n";
    }

    return sensorData.str();
}

