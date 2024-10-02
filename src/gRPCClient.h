#ifndef GRPCCLIENT_H
#define GRPCCLIENT_H

#include <grpcpp/grpcpp.h>
#include "sensor.grpc.pb.h"
#include <memory>
#include <sstream>

class gRPCClient {
public:
    gRPCClient(std::shared_ptr<grpc::Channel> channel);

    std::string GetSensorsList(const std::string& sensor_type);

private:
    std::unique_ptr<sensor::SensorService::Stub> stub_;
};

#endif // GRPCCLIENT_H
