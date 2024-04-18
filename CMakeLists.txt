cmake_minimum_required(VERSION 3.22)
project(godbolt)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
		cmake_policy(SET CMP0135 NEW)
endif()
include_directories(include/)
include(FetchContent)
FetchContent_Declare(
  httplib
  URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.15.3.tar.gz
)
FetchContent_MakeAvailable(httplib)


FetchContent_Declare(
  nlohmann_json
  URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.tar.gz
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  cxxopts
  URL https://github.com/jarro2783/cxxopts/archive/refs/tags/v3.2.0.tar.gz
)
FetchContent_MakeAvailable(cxxopts)


add_executable(godbolt src/godbolt.cpp)
target_link_libraries(godbolt PRIVATE cxxopts nlohmann_json::nlohmann_json httplib ssl crypto)
# target_link_libraries(godbolt PRIVATE cpp_httplib)
