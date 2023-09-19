#include <httplib.h>
#include <string>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 3) { // A very basic check to ensure there's enough data for our test
        return 0;
    }

    httplib::Server svr;

    // We'll start a dummy server that responds to every request
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello World!", "text/plain");
    });

    // Now, using the fuzzer's input, we'll send a request to our dummy server
    httplib::Client cli("localhost", 8080);

    // The first byte of the data will decide the type of request
    switch (data[0] % 4) {
        case 0:
            cli.Get(reinterpret_cast<const char*>(data + 1), size - 1);
            break;
        case 1:
            cli.Post("/", std::string(reinterpret_cast<const char*>(data + 1), size - 1), "text/plain");
            break;
        case 2:
            cli.Put("/", std::string(reinterpret_cast<const char*>(data + 1), size - 1), "text/plain");
            break;
        case 3:
            cli.Delete("/", std::string(reinterpret_cast<const char*>(data + 1), size - 1));
            break;
    }

    return 0; 
}
