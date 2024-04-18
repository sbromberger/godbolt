#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace godbolt {
using json = nlohmann::json;
inline const std::string BASE_URL = "https://godbolt.org";
inline const std::string DEFAULT_COMPILER = "cclang1810";

// {
//   "sessions": [
//     {
//       "id": 1,
//       "language": "c++",
//       "source": "int main() { return 42; }",
//       "compilers": [
//         {
//           "id": "g82",
//           "options": "-O3"
//         }
//       ],
//       "executors": [
//         {
//           "arguments": "arg1",
//           "compiler": {
//               "id": "g92",
//               "libs": [],
//               "options": "-O3"
//           },
//           "stdin": ""
//         }
//       ]
//     }
//   ]
// }
using Compiler = std::unordered_map<std::string, std::string>;
struct GodboltSession {
  int id;
  std::string language{"c++"};
  std::string source;
  std::vector<Compiler> compilers = {{{"id", DEFAULT_COMPILER}}};

  GodboltSession(int id, std::string source)
      : id(id), source(std::move(source)){};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GodboltSession, id, language, source,
                                   compilers)
using GBSessions = std::vector<GodboltSession>;

json make_json(const GBSessions &g) {
  std::cout << "in make_json, g.size = " << g.size() << "\n";
  json j = std::unordered_map<std::string, GBSessions>{{"sessions", g}};
  std::cout << "returning\n";
  return j;
}

inline std::string post_it(const GBSessions &g) {
  auto j = make_json(g);
  std::cout << "past make_json\n";

  std::cout << j << "\n\n";
  // exit(0);
  httplib::Client cli(BASE_URL);

  const auto *url = "/api/shortener/";
  std::cout << "POST to " << url << "\n";
  auto post = j.dump();
  std::cout << "post = " << post << "\n";
  std::cout << "post dump\n";
  auto res = cli.Post(url, post, "application/json");
  std::cout << "post post\n";
  if (res == nullptr) {
    std::cerr << "res is null; exiting\n";
    exit(1);
  }
  std::cout << "status = " << res->status << "\n";

  if (res->status != 200) {
    std::cerr << "Request returned " << res->status << "; exiting\n";
    exit(1);
  }
  return res->body;
}
} // namespace godbolt
