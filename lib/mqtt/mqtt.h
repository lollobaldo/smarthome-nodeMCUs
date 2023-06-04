#include <Arduino.h>
#include <PubSubClient.h>

#include <vector>

namespace mqtt {
    typedef void (*callbackPointer_t) (const char* topic, const char* payload);

    extern PubSubClient client;
    void setup(const char* clientName);
    void subscribe(const char* topic, callbackPointer_t callback);
    void loop();
}
