#include "gRPCServer.h"

gRPCServer::gRPCServer(SensorsController& controller, const std::string& server_address)
    : sensorsController(controller), server_address(server_address) {}

void gRPCServer::Run()
{
    std::cout << "gRPC server is starting at " << server_address << "..." << std::endl;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    builder.RegisterService(this);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "gRPC server has started." << std::endl;

    server->Wait();
}

grpc::Status gRPCServer::GetSensorData(grpc::ServerContext* context,
                                        const sensor::SensorRequest* request,
                                        sensor::SensorData* response)
{
    try
    {
        std::string sensorType = request->sensor_type();
        std::string data = sensorsController.getSpecificSensorData(sensorType);

        if (data.empty())
        {
            return grpc::Status(grpc::NOT_FOUND, "Sensor data not found");
        }
        else
        {
            response->set_data(data);
            return grpc::Status::OK;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error getting specific sensor data: " << e.what() << std::endl;
        return grpc::Status(grpc::INTERNAL, "Error retrieving data");
    }
}

grpc::Status gRPCServer::ListAllSensors(grpc::ServerContext* context,
                                         const sensor::Empty* request,
                                         sensor::SensorList* response)
{
    auto sensorsList = sensorsController.getCreatedSensorsList();
    for (const auto& sensor : sensorsList)
    {
        response->add_sensors(sensor);
    }
    return grpc::Status::OK;
}

grpc::Status gRPCServer::CalculateSensorData(grpc::ServerContext* context, const sensor::SensorData* request, sensor::SensorData* response)
{
    std::string sensorType = request->sensortype();

    auto customData = sensorsController.calculateSensorData(sensorType);

    response->set_averagereading(customData.averageReading);
    response->set_minreading(customData.minReading);
    response->set_maxreading(customData.maxReading);
    response->set_sensortype(customData.sensortype);

    if (customData.averageReading == 0.0 &&
        customData.minReading == std::numeric_limits<double>::max() &&
        customData.maxReading == std::numeric_limits<double>::min())
    {
    }

    return grpc::Status::OK;
}