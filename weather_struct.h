//
// Created by Tobias on 18.05.2024.
//

#ifndef WEATHERAPP_WEATHER_STRUCT_H
#define WEATHERAPP_WEATHER_STRUCT_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using std::cout;
using std::endl;

struct Weather {
    // Popis mraků
    std::string description = "Not available";
    // Teplota v C°
    std::string temp = "Not available";
    std::string feels_like = "Not available";
    // Tlak v HPa
    std::string pressure = "Not available";
    // Vlhkost v %
    std::string humidity = "Not available";
    // viditelnost na kilometry
    std::string visibility = "Not available";
    // rychlost větru v km/h
    std::string wind_speed = "Not available";
    // zataženo v %
    std::string cloudiness = "Not available";
    // Formátovaný čas kdy byly data získány
    std::string data_time = "Not available";
    // Město odkud jsou informace
    std::string name = "Not available";
    // Kvalita vzduchu - Good až Poor
    std::string air_quality = "Not available";
};

struct Astronomy {
    // Formátovaný jako běžný čas co je rovnou čitelný, není to unix
    std::string local_time = "Not available";
    std::string sunrise = "Not available";
    std::string sunset = "Not available";
    std::string moonrise = "Not available";
    std::string moonset = "Not available";
    // Text toho v jaké fázi je měsíc
    std::string moon_phase = "Not available";
    // Jak moc je měsíc osvětlen v % 0 - 100
    std::string moon_illumination = "Not available";
    // Na tento jediný řádek musím brát API z U.S. Naval Observatory
    std::string next_full_moon = "Not available";
};

[[maybe_unused]] void print_weather(const Weather &weather);

[[maybe_unused]] void print_astronomy(const Astronomy &astronomy);

std::string format_float(float num);

std::string unix_to_string(time_t time);

std::string decide_air_quality(double co, double no2, double o3, double so2, double pm2_5, double pm10);

#endif //WEATHERAPP_WEATHER_STRUCT_H
