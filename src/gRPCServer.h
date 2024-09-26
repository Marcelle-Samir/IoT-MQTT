#ifndef GRPCSERVER_H
#define GRPCSERVER_H

#include <grpcpp/grpcpp.h>
#include "sensor.grpc.pb.h"

#include "SensorsController.h"

class gRPCServer : public sensor::SensorService::Service
{
public:
    gRPCServer(SensorsController& controller, const std::string& server_address);
    void Run();

    grpc::Status GetSensorData(grpc::ServerContext* context,
                                const sensor::SensorRequest* request,
                                sensor::SensorData* response) override;

    grpc::Status ListAllSensors(grpc::ServerContext* context,
                                 const sensor::Empty* request,
                                 sensor::SensorList* response) override;

    grpc::Status CalculateSensorData(grpc::ServerContext* context,
                                const sensor::SensorData* request,
                                sensor::SensorData* response) override;

private:
    SensorsController& sensorsController;
    std::string server_address;
};

#endif // GRPCSERVER_H
