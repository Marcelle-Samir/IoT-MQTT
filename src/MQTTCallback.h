#ifndef MQTTCALLBACK_H
#define MQTTCALLBACK_H

#include <mqtt/async_client.h>

class SensorsController;

class MQTTCallback : public virtual mqtt::callback, public virtual mqtt::iaction_listener
{
public:
    void message_arrived(mqtt::const_message_ptr msg) override;

    void on_failure(const mqtt::token& tok) override;
    void on_success(const mqtt::token& tok) override;
    void connected(const std::string& cause) override;
    void connection_lost(const std::string& cause) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

private:
    void storeSensorReading(const std::string& sensorType, double reading);
};

#endif // MQTTCALLBACK_H