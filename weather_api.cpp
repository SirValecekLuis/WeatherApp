//
// Created by Tobias on 18.05.2024.
//

#include "weather_api.h"

std::optional<Weather> get_weather(CURL *curl, const std::string &city) {
    const std::string url =
            "https://api.weatherapi.com/v1/current.json?key=" + WEATHER_API_KEY + "&q=" + city + "&aqi=yes";
    auto response = send_request(curl, url);

    if (!response.has_value()) return std::nullopt;

    Weather weather;

    rapidjson::Document document;
    document.Parse(response.value().c_str());

    if (document.HasParseError()) return std::nullopt;

    if (document.HasMember("current") && document["current"].IsObject()) {
        const rapidjson::Value &current = document["current"];

        if (current.HasMember("condition") &&
            current["condition"].IsObject() &&
            current["condition"].HasMember("text") && current["condition"]["text"].IsString()) {
            weather.description = current["condition"]["text"].GetString();
        }

        if (current.HasMember("temp_c") && current["temp_c"].IsFloat()) {
            weather.temp = format_float(current["temp_c"].GetFloat());
        }

        if (current.HasMember("feelslike_c") && current["feelslike_c"].IsFloat()) {
            weather.feels_like = format_float(current["feelslike_c"].GetFloat());
        }

        if (current.HasMember("pressure_mb") && current["pressure_mb"].IsFloat()) {
            weather.pressure = format_float(current["pressure_mb"].GetFloat());
        }

        if (current.HasMember("humidity") && current["humidity"].IsInt()) {
            weather.humidity = std::to_string(current["humidity"].GetInt());
        }

        if (current.HasMember("vis_km") && current["vis_km"].IsFloat()) {
            weather.visibility = format_float(current["vis_km"].GetFloat());
        }

        if (current.HasMember("wind_kph") && current["wind_kph"].IsFloat()) {
            weather.wind_speed = format_float(current["wind_kph"].GetFloat());
        }

        if (current.HasMember("cloud") && current["cloud"].IsInt()) {
            weather.cloudiness = std::to_string(current["cloud"].GetInt());
        }

        if (current.HasMember("last_updated_epoch") && current["last_updated_epoch"].IsInt()) {
            weather.data_time = unix_to_string(current["last_updated_epoch"].GetInt());
        }
    }

    if (document.HasMember("location") && document["location"].IsObject()) {
        const rapidjson::Value &location = document["location"];

        if (location.HasMember("name")) {
            weather.name = location["name"].GetString();
        }
    }


    double co = -1, no2 = -1, o3 = -1, so2 = -1, pm2_5 = -1, pm10 = -1;

    if (document.HasMember("current") && document["current"].IsObject()) {
        const rapidjson::Value &current = document["current"];

        if (current.HasMember("air_quality") && current["air_quality"].IsObject()) {
            const rapidjson::Value &air_quality = current["air_quality"];

            if (air_quality.HasMember("co") && air_quality["co"].IsFloat()) {
                co = air_quality["co"].GetFloat();
            }

            if (air_quality.HasMember("no2") && air_quality["no2"].IsFloat()) {
                no2 = air_quality["no2"].GetFloat();
            }

            if (air_quality.HasMember("o3") && air_quality["o3"].IsFloat()) {
                o3 = air_quality["o3"].GetFloat();
            }

            if (air_quality.HasMember("so2") && air_quality["so2"].IsFloat()) {
                so2 = air_quality["so2"].GetFloat();
            }

            if (air_quality.HasMember("pm2_5") && air_quality["pm2_5"].IsFloat()) {
                pm2_5 = air_quality["pm2_5"].GetFloat();
            }

            if (air_quality.HasMember("pm10") && air_quality["pm10"].IsFloat()) {
                pm10 = air_quality["pm10"].GetFloat();
            }
        }
    }

    weather.air_quality = decide_air_quality(co, no2, o3, so2, pm2_5, pm10);

    return {weather};
}

std::optional<Astronomy> get_astronomy(CURL *curl, const std::string &city) {
    std::time_t timestamp = time(&timestamp);
    std::tm tm_struct = *std::localtime(&timestamp);

    const char *format = "%Y-%m-%d";
    char dt[20];
    std::size_t size = std::strftime(dt, 20, format, &tm_struct);

    if (size == 0) return std::nullopt;

    const std::string url =
            "https://api.weatherapi.com/v1/astronomy.json?key=" + WEATHER_API_KEY + "&q=" + city + "&dt=" + dt;

    auto response = send_request(curl, url);

    if (!response.has_value()) return std::nullopt;

    rapidjson::Document document;
    document.Parse(response.value().c_str());

    if (document.HasParseError()) return std::nullopt;

    Astronomy astronomy;

    if (document.HasMember("location") && document["location"].IsObject()) {
        const rapidjson::Value &location = document["location"];

        if (location.HasMember("localtime")) {
            astronomy.local_time = location["localtime"].GetString();
        }
    }

    if (document.HasMember("astronomy") && document["astronomy"].IsObject()) {
        const rapidjson::Value &astronomyObj = document["astronomy"];

        if (astronomyObj.HasMember("astro") && astronomyObj["astro"].IsObject()) {
            const rapidjson::Value &astro = astronomyObj["astro"];

            if (astro.HasMember("sunrise")) {
                astronomy.sunrise = astro["sunrise"].GetString();
            }

            if (astro.HasMember("sunset")) {
                astronomy.sunset = astro["sunset"].GetString();
            }

            if (astro.HasMember("moonrise")) {
                astronomy.moonrise = astro["moonrise"].GetString();
            }

            if (astro.HasMember("moonset")) {
                astronomy.moonset = astro["moonset"].GetString();
            }

            if (astro.HasMember("moon_phase")) {
                astronomy.moon_phase = astro["moon_phase"].GetString();
            }

            if (astro.HasMember("moon_illumination")) {
                astronomy.moon_illumination = std::to_string(astro["moon_illumination"].GetInt());
            }
        }
    }

    auto full_moon_date = get_full_moon(curl);
    if (full_moon_date.has_value()){
        astronomy.next_full_moon = full_moon_date.value();
    }

    return {astronomy};
}

std::optional<std::string> get_full_moon(CURL *curl) {
    std::time_t timestamp = time(&timestamp);
    std::tm tm_struct = *std::localtime(&timestamp);

    const char *format = "%Y-%m-%d";
    char dt[20];
    std::size_t size = std::strftime(dt, 20, format, &tm_struct);
    std::string dt_str(dt);

    if (size == 0) return std::nullopt;

    const std::string url = "https://aa.usno.navy.mil/api/moon/phases/date?date=" + dt_str + "&nump=4&id=SVLUIS";

    auto response = send_request(curl, url);

    if (!response.has_value()) return std::nullopt;

    rapidjson::Document document;
    document.Parse(response.value().c_str());

    if (document.HasParseError()) return std::nullopt;


    if (!document.HasMember("phasedata") || !document["phasedata"].IsArray()) {
        return std::nullopt;
    }

    int day = 0;
    int month = 0;
    int year = 0;
    const rapidjson::Value &phase_data = document["phasedata"];
    for (rapidjson::SizeType i = 0; i < phase_data.Size(); i++) {
        const rapidjson::Value &phase = phase_data[i];
        if (phase.HasMember("phase") && phase["phase"].IsString()) {
            // Když najdu fázi úplňku, vezmu si datum a zformátuji to
            if (strcmp(phase["phase"].GetString(), "Full Moon") == 0) {
                day = phase["day"].GetInt();
                month = phase["month"].GetInt();
                year = phase["year"].GetInt();
            }
        }
    }

    std::string full_moon_date = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);

    return {full_moon_date};
}
