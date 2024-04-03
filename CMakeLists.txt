cmake_minimum_required(VERSION 3.27)
project(WeatherApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

add_executable(WeatherApp main.cpp
        weather.h)

find_package(unofficial-curlpp CONFIG REQUIRED)
target_link_libraries(WeatherApp PRIVATE unofficial::curlpp::curlpp)

find_package(RapidJSON CONFIG REQUIRED)
target_link_libraries(WeatherApp PRIVATE rapidjson)