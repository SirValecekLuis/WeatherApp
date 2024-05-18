//
// Created by Tobias on 18.05.2024.
//

#ifndef WEATHERAPP_WEATHER_API_H
#define WEATHERAPP_WEATHER_API_H

#include <ctime>

#include "api_call.h"
#include "open_weather.h"
#include "weather_struct.h"

const std::string WEATHER_API_KEY = "637bc21a04544203a27130948241805";

std::optional<Weather> get_weather(CURL *curl, const std::string &city);

std::optional<Astronomy> get_astronomy(CURL *curl, const std::string &city);

#endif //WEATHERAPP_WEATHER_API_H
