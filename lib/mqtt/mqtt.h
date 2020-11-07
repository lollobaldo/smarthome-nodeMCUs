#include <wifi.h>

#include <Arduino.h>
#include <PubSubClient.h>

#include <vector>

typedef void callbackType (char* topic, byte* payload, unsigned int length);

namespace mqtt {
    extern std::vector<const char*> topics;
    extern const char* cLog;

    extern PubSubClient client;

    void setup(const char* clientName, std::vector<const char*> subscriptions, callbackType callback);
    void loop();
}
