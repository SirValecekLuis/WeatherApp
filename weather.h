//
// Created by Tobias on 02.04.2024.
//

#ifndef WEATHERAPP_WEATHER_H
#define WEATHERAPP_WEATHER_H

#include <curl/curl.h>
#include <rapidjson/document.h>

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <optional>

using std::cout;
using std::endl;

struct Weather {
    std::string lon;
    std::string lat;
    std::string description;
    std::string temp;
    std::string feels_like;
    std::string temp_min;
    std::string temp_max;
    std::string pressure;
    std::string humidity;
    std::string visibility;
    std::string wind_speed;
    std::string cloudiness;
    std::string data_time;
    std::string sunrise;
    std::string sunset;
    std::string timezone;
    std::string name;
};

[[maybe_unused]] void print_weather(const Weather &weather);

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);

std::optional<std::string> send_request(CURL *curl, std::string &url);

std::optional<std::pair<std::string, std::string>>
get_lat_lon(CURL *curl, std::string &api_key, std::string &city);

std::optional<std::string>
get_weather(CURL *curl, std::string &api_key, std::string &city, std::string &lat, std::string &lon);

std::optional<std::string> get_air_quality(CURL *curl, std::string &api_key, std::string &lat, std::string &lon);

#endif //WEATHERAPP_WEATHER_H
