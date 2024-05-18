cmake_minimum_required(VERSION 3.27)
project(WeatherApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(WeatherApp main.cpp
        open_weather.h
        application.cpp
        application.h
        api_call.cpp
        api_call.h
        open_weather.cpp
        weather_api.cpp
        weather_api.h
        weather_struct.h
        weather_struct.cpp
)

find_package(Qt6Widgets CONFIG REQUIRED)
target_link_libraries(WeatherApp PRIVATE Qt6::Widgets)

find_package(unofficial-curlpp CONFIG REQUIRED)
target_link_libraries(WeatherApp PRIVATE unofficial::curlpp::curlpp)

find_package(RapidJSON CONFIG REQUIRED)
target_link_libraries(WeatherApp PRIVATE rapidjson)

