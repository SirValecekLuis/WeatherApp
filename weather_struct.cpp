//
// Created by Tobias on 18.05.2024.
//

#include <cmath>

#include "weather_struct.h"


[[maybe_unused]] void print_weather(const Weather &weather) {
    cout << "City: " << weather.name << endl;
    cout << "description: " << weather.description << endl;
    cout << "temp: " << weather.temp << endl;
    cout << "feels_like: " << weather.feels_like << endl;
    cout << "pressure: " << weather.pressure << endl;
    cout << "humidity: " << weather.humidity << endl;
    cout << "visibility: " << weather.visibility << endl;
    cout << "wind_speed: " << weather.wind_speed << endl;
    cout << "cloudiness: " << weather.cloudiness << endl;
    cout << "data_time: " << weather.data_time << endl;
    cout << "air_quality: " << weather.air_quality << endl;
}

[[maybe_unused]] void print_astronomy(const Astronomy &astronomy) {
    cout << "Local Time: " << astronomy.local_time << endl;
    cout << "Sunrise: " << astronomy.sunrise << endl;
    cout << "Sunset: " << astronomy.sunset << endl;
    cout << "Moonrise: " << astronomy.moonrise << endl;
    cout << "Moonset: " << astronomy.moonset << endl;
    cout << "Moon Phase: " << astronomy.moon_phase << endl;
    cout << "Moon Illumination: " << astronomy.moon_illumination << endl;
    cout << "Is Moon Up: " << astronomy.is_moon_up << endl;
    cout << "Is Sun Up: " << astronomy.is_sun_up << endl;
    cout << "When is next full moon: " << astronomy.next_full_moon << endl;
}

std::string format_float(float num) {
    // Toto je asi dobrý způsob jak porovnat, že je číslo stejné jako celé číslo
    const float epsilon = 1e-5f;  // Tolerance pro porovnání

    num = std::round(num * 10.0f) / 10.0f;

    if (std::fabs(num - std::round(num)) < epsilon) {
        return std::to_string(static_cast<int>(num));
    }

    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << num;
    return stream.str();
}

std::string unix_to_string(time_t time) {
    std::tm *tm_struct = std::localtime(&time);

    auto txt_lambda = [](int time) -> std::string {
        return time < 10 ? "0" + std::to_string(time) : std::to_string(time);
    };

    std::string formated_time =
            "Last update at " + txt_lambda(tm_struct->tm_hour) + ":" + txt_lambda(tm_struct->tm_min) +
            ":" + txt_lambda(tm_struct->tm_sec);

    return formated_time;
}

std::string decide_air_quality(double co, double no2, double o3, double so2, double pm2_5, double pm10) {
    std::string air_quality;
    if (co == -1 && no2 == -1 && o3 == -3 && so2 == -1 && pm2_5 == -1 && pm10 == -1) {
        air_quality = "Not found";
        return air_quality;
    }

    if (so2 < 20 && no2 < 40 && pm10 < 20 && pm2_5 < 10 && o3 < 60 && co < 4400) {
        air_quality = "Good";
    } else if (so2 < 80 && no2 < 70 && pm10 < 50 && pm2_5 < 25 && o3 < 100 && co < 9400) {
        air_quality = "Fair";
    } else if (so2 < 250 && no2 < 150 && pm10 < 100 && pm2_5 < 50 && o3 < 140 && co < 12400) {
        air_quality = "Moderate";
    } else if (so2 < 350 && no2 < 200 && pm10 < 200 && pm2_5 < 75 && o3 < 180 && co < 15400) {
        air_quality = "Poor";
    } else {
        air_quality = "Very poor";
    }

    return air_quality;
}