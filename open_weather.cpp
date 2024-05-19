//
// Created by Tobias on 18.05.2024.
//

#include "open_weather.h"

std::optional <std::pair<std::string, std::string>>
get_lat_lon(CURL *curl, const std::string &city) {
    // GEO http://api.openweathermap.org/geo/1.0/direct?q={city name},{state code},{country code}&limit={limit}&appid={API key}
    const std::string url =
            "https://api.openweathermap.org/geo/1.0/direct?q=" + city + "&limit=1&appid=" + OPEN_WEATHER_KEY;

    auto response = send_request(curl, url);
    if (!response.has_value()) return std::nullopt;

    rapidjson::Document document;
    document.Parse(response.value().c_str());

    if (document.HasParseError()) return std::nullopt;

    auto object = document[0].GetObj();
    auto lat = object["lat"].IsDouble();
    auto lon = object["lon"].IsDouble();
    if (lon && lat) {
        return {{std::to_string(object["lat"].GetFloat()), std::to_string(object["lon"].GetFloat())}};
    }

    return std::nullopt;
}

std::optional <Weather>
get_weather(CURL *curl, const std::string &city, const std::string &lat,
            const std::string &lon) {
    // Weather data https://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid={API key}
    const std::string url =
            "https://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + lon + "&appid=" +
            OPEN_WEATHER_KEY + "&units=metric" + "&lang=en";

    auto response = send_request(curl, url);

    if (!response.has_value()) return std::nullopt;

    rapidjson::Document document;
    document.Parse(response.value().c_str());

    if (document.HasParseError()) return std::nullopt;

    Weather weather;

    weather.name = city;

    if (document.HasMember("weather") && document["weather"].IsArray() && !document["weather"].Empty()) {
        const rapidjson::Value &weatherArray = document["weather"];
        const rapidjson::Value &weatherItem = weatherArray[0];
        weather.description = (weatherItem.HasMember("description") && weatherItem["description"].IsString())
                              ? weatherItem["description"].GetString() : weather.description;
    }

    if (document.HasMember("main") && document["main"].IsObject()) {
        const rapidjson::Value &main = document["main"];
        weather.temp = (main.HasMember("temp") && main["temp"].IsFloat()) ? format_float(main["temp"].GetFloat())
                                                                          : weather.temp;
        weather.feels_like = (main.HasMember("feels_like") && main["feels_like"].IsFloat()) ? format_float(
                main["feels_like"].GetFloat()) : weather.feels_like;
        weather.pressure = (main.HasMember("pressure") && main["pressure"].IsInt()) ? std::to_string(
                main["pressure"].GetInt()) : weather.pressure;
        weather.humidity = (main.HasMember("humidity") && main["humidity"].IsInt()) ? std::to_string(
                main["humidity"].GetInt()) : weather.humidity;
    }

    weather.visibility = (document.HasMember("visibility") && document["visibility"].IsInt()) ? std::to_string(
            document["visibility"].GetInt() / 1000) : weather.visibility;

    if (document.HasMember("wind") && document["wind"].IsObject()) {
        const rapidjson::Value &wind = document["wind"];
        weather.wind_speed = (wind.HasMember("speed") && wind["speed"].IsFloat()) ? format_float(
                wind["speed"].GetFloat()) : weather.wind_speed;
    }

    if (document.HasMember("clouds") && document["clouds"].IsObject()) {
        const rapidjson::Value &clouds = document["clouds"];
        weather.cloudiness = (clouds.HasMember("all") && clouds["all"].IsInt()) ? std::to_string(clouds["all"].GetInt())
                                                                                : weather.cloudiness;
    }

    weather.data_time = (document.HasMember("dt") && document["dt"].IsInt()) ? unix_to_string(document["dt"].GetInt())
                                                                             : weather.data_time;

    auto air_quality = get_air_quality(curl, lat, lon);
    weather.air_quality = air_quality.has_value() ? air_quality.value() : weather.air_quality;

    return {weather};
}

std::optional <std::string>
get_air_quality(CURL *curl, const std::string &lat, const std::string &lon) {
    // http://api.openweathermap.org/data/2.5/air_pollution?lat={lat}&lon={lon}&appid={API key}
    // https://openweathermap.org/api/air-pollution
    const std::string url =
            "https://api.openweathermap.org/data/2.5/air_pollution?lat=" + lat + "&lon=" + lon + "&appid=" +
            OPEN_WEATHER_KEY;

    auto response = send_request(curl, url);

    if (!response.has_value()) return std::nullopt;

    rapidjson::Document document;
    document.Parse(response.value().c_str());

    if (document.HasParseError()) return std::nullopt;

    double co = -1, no2 = -1, o3 = -1, so2 = -1, pm2_5 = -1, pm10 = -1;

    const rapidjson::Value &list = document["list"];
    if (list.IsArray() && !list.Empty() && list[0].HasMember("components")) {
        const rapidjson::Value &components = list[0]["components"];

        if (components.HasMember("co") && components["co"].IsDouble()) {
            co = components["co"].GetDouble();
        }

        if (components.HasMember("no2") && components["no2"].IsDouble()) {
            no2 = components["no2"].GetDouble();
        }

        if (components.HasMember("o3") && components["o3"].IsDouble()) {
            o3 = components["o3"].GetDouble();
        }

        if (components.HasMember("so2") && components["so2"].IsDouble()) {
            so2 = components["so2"].GetDouble();
        }
        if (components.HasMember("pm2_5") && components["pm2_5"].IsDouble()) {
            pm2_5 = components["pm2_5"].GetDouble();
        }

        if (components.HasMember("pm10") && components["pm10"].IsDouble()) {
            pm10 = components["pm10"].GetDouble();
        }
    }

    return {decide_air_quality(co, no2, o3, so2, pm2_5, pm10)};
}