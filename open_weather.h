//
// Created by Tobias on 02.04.2024.
//

#ifndef WEATHERAPP_OPEN_WEATHER_H
#define WEATHERAPP_OPEN_WEATHER_H

#include <curl/curl.h>
#include <rapidjson/document.h>

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <optional>

#include "api_call.h"
#include "weather_struct.h"

const std::string OPEN_WEATHER_KEY = "cdc4242f13de51119450c25f18455def";

using std::cout;
using std::endl;

std::optional<std::pair<std::string, std::string>>
get_lat_lon(CURL *curl, const std::string &city);

std::optional<Weather>
get_weather(CURL *curl, const std::string &city, const std::string &lat,
            const std::string &lon);

std::optional<std::string> get_air_quality(CURL *curl, const std::string &lat, const std::string &lon);

#endif //WEATHERAPP_OPEN_WEATHER_H
