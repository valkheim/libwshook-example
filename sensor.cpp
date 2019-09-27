#include <iostream>
#include "json/json.h"
#include "libwshook/libwshook.hpp"

int ping(std::string target);

Json::Value generateMetrics(int const us, bool const alive) {
  Json::Value metrics;

  metrics[0]["channel"] = "Time";
  metrics[0]["unit"] = "ms";
  metrics[0]["value"] = std::to_string(us * 0.001);
  metrics[1]["channel"] = "Alive";
  metrics[1]["unit"] = "boolean";
  metrics[1]["value"] = [alive](){ return alive ? "1" : "0"; }();

  return metrics;
}

Json::Value foo(Json::Value const &job) {
  try {
    int const us = ping(job["settings"]["target"].asString());
    if (auto alive = (us > 0); alive)
      return generateMetrics(us, alive);
    else
      return generateMetrics(0, alive);
  } catch(...) {
    std::cerr << "Cannot ping" << std::endl;
    throw;
  }
}

int main(int argc, char *argv[])
{
  try {
    libwshook::wshook wshook(
        "127.0.0.1",
        4910,
        "0cb382d6-d852-4807-a210-106be9dd4d56",
        "YYOfYw5P8rQvYemJBiPqDp9A920v26hu4w7V0l7O8c"
        );

    // register PULL handler
    wshook.hook("PULL", &foo);

    wshook.run();
  } catch(...) {
    std::cerr << "Failed" << std::endl;
    return 1;
  }
}
