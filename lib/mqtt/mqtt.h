#include <wifi.h>

#include <Arduino.h>
#include <PubSubClient.h>

typedef void callbackType (char* topic, byte* payload, unsigned int length);

namespace mqtt {
    extern const char* cDebug;
    extern const char* cInput;
    extern const char* cLog;

    extern PubSubClient client;

    void setup(const char* clientName, const char* channelInput, callbackType callback);
    void loop();
}
