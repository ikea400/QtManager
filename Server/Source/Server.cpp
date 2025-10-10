#include <drogon/drogon.h>

int main()
{
    // Set HTTP listener address and port
    drogon::app().loadConfigFile("config.json");
    // Run HTTP framework, the method will block in the internal event loop
    drogon::app().run();

    //drogon::app().addListener("127.0.0.1", 8848).run();
    return 0;
}