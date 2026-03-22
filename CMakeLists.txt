cmake_minimum_required(VERSION 3.16)

project(real_time_lightning_visualization LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 1.0.1
	GIT_SHALLOW TRUE
)

# nlohmann JSON 
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    GIT_SHALLOW TRUE
)

# IXWebSocket
FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG v11.4.5
    GIT_SHALLOW TRUE
)

# IXWebSocket uses these cache options to enable TLS backends.
set(USE_TLS ON CACHE BOOL "Enable TLS support for IXWebSocket" FORCE)
set(USE_OPEN_SSL ON CACHE BOOL "Use OpenSSL backend for IXWebSocket TLS" FORCE)
set(USE_MBED_TLS OFF CACHE BOOL "Disable mbedTLS backend for IXWebSocket TLS" FORCE)

FetchContent_MakeAvailable(glm nlohmann_json ixwebsocket)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

add_subdirectory(include)
add_subdirectory(src)
