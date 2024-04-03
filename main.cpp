#include "weather.h"

[[maybe_unused]] void print_weather(const Weather &weather) {
    cout << "City: " << weather.name << endl;
    cout << "Lon: " << weather.lon << endl;
    cout << "lat: " << weather.lat << endl;
    cout << "description: " << weather.description << endl;
    cout << "temp: " << weather.temp << endl;
    cout << "feels_like: " << weather.feels_like << endl;
    cout << "temp_min: " << weather.temp_min << endl;
    cout << "temp_max: " << weather.temp_max << endl;
    cout << "pressure: " << weather.pressure << endl;
    cout << "humidity: " << weather.humidity << endl;
    cout << "visibility: " << weather.visibility << endl;
    cout << "wind_speed: " << weather.wind_speed << endl;
    cout << "cloudiness: " << weather.cloudiness << endl;
    cout << "data_time: " << weather.data_time << endl;
    cout << "sunrise: " << weather.sunrise << endl;
    cout << "sunset: " << weather.sunset << endl;
    cout << "timezone: " << weather.timezone << endl;
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // https://gist.github.com/alghanmi/c5d7b761b2c9ab199157
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

std::optional<std::string> send_request(CURL *curl, const std::string &url) {
    std::string read_buffer;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    curl_easy_perform(curl);    // Řádek, který spouští celý dotaz
    int code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code); // Kontrola HTTP odpovědi

    // Tady musí vracet "OK" což je 200 a string nesmí být prázdný, jinak to znamená, že HTTP dotaz nevrátil nic
    if (code != 200 || read_buffer.empty() || read_buffer == "[]") {
        return std::nullopt;
    }

    return {read_buffer};
}

std::optional<std::pair<std::string, std::string>>
get_lat_lon(CURL *curl, const std::string &api_key, const std::string &city) {
    // GEO http://api.openweathermap.org/geo/1.0/direct?q={city name},{state code},{country code}&limit={limit}&appid={API key}
    const std::string url = "https://api.openweathermap.org/geo/1.0/direct?q=" + city + "&limit=1&appid=" + api_key;

    auto response = send_request(curl, url);
    if (!response.has_value()) return std::nullopt;

    rapidjson::Document document;
    document.Parse(response.value().c_str());

    if (document.HasParseError()) {
        return std::nullopt;
    }

    auto object = document[0].GetObj();
    auto lat = object["lat"].IsDouble();
    auto lon = object["lon"].IsDouble();
    if (lon && lat) {
        return {{std::to_string(object["lat"].GetDouble()), std::to_string(object["lon"].GetDouble())}};
    }

    return std::nullopt;
}

std::optional<Weather>
get_weather(CURL *curl, const std::string &api_key, const std::string &city, const std::string &lat,
            const std::string &lon) {
    // Weather data https://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid={API key}
    const std::string url =
            "https://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + lon + "&appid=" + api_key +
            "&units=metric" + "&lang=en";

    auto response = send_request(curl, url);

    if (!response.has_value()) return std::nullopt;

    rapidjson::Document document;
    document.Parse(response.value().c_str());

    if (document.HasParseError()) {
        return std::nullopt;
    }

    Weather weather;

    // cout << response.value() << endl;

    // cout << document.GetObj().FindMember("lon")->name.GetString() << endl;

    weather.name = city;
    weather.lon = std::to_string(document["coord"]["lon"].GetFloat());
    weather.lat = std::to_string(document["coord"]["lat"].GetFloat());
    weather.description = document["weather"][0]["description"].GetString();
    weather.temp = std::to_string(document["main"]["temp"].GetFloat());
    weather.feels_like = std::to_string(document["main"]["feels_like"].GetFloat());
    weather.temp_min = std::to_string(document["main"]["temp_min"].GetFloat());
    weather.temp_max = std::to_string(document["main"]["temp_max"].GetFloat());
    weather.pressure = std::to_string(document["main"]["pressure"].GetInt());
    weather.humidity = std::to_string(document["main"]["humidity"].GetInt());
    weather.visibility = std::to_string(document["visibility"].GetInt());
    weather.wind_speed = std::to_string(document["wind"]["speed"].GetFloat());
    weather.cloudiness = std::to_string(document["clouds"]["all"].GetInt());
    weather.data_time = std::to_string(document["dt"].GetInt());
    weather.sunrise = std::to_string(document["sys"]["sunrise"].GetInt());
    weather.sunset = std::to_string(document["sys"]["sunset"].GetInt());
    weather.timezone = std::to_string(document["timezone"].GetInt() / 3600);

    return {weather};
}

std::optional<std::string>
get_air_quality(CURL *curl, const std::string &api_key, const std::string &lat, const std::string &lon) {
    // http://api.openweathermap.org/data/2.5/air_pollution?lat={lat}&lon={lon}&appid={API key}
    // https://openweathermap.org/api/air-pollution
    const std::string url =
            "https://api.openweathermap.org/data/2.5/air_pollution?lat=" + lat + "&lon=" + lon + "&appid=" +
            api_key;

    auto response = send_request(curl, url);

    if (!response.has_value()) return std::nullopt;

    rapidjson::Document document;
    document.Parse(response.value().c_str());

    if (document.HasParseError()) {
        return std::nullopt;
    }

    cout << response.value() << endl;

    std::string air_quality;
    auto co = document["list"][0]["components"]["co"].GetDouble();
    auto no2 = document["list"][0]["components"]["no2"].GetDouble();
    auto o3 = document["list"][0]["components"]["o3"].GetDouble();
    auto so2 = document["list"][0]["components"]["so2"].GetDouble();
    auto pm2_5 = document["list"][0]["components"]["pm2_5"].GetDouble();
    auto pm10 = document["list"][0]["components"]["pm10"].GetDouble();

    if (so2 < 20 && no2 < 40 && pm10 < 20 && pm2_5 < 10 && o3 < 60 && co < 4400){
        air_quality = "Good";
    } else if (so2 < 80 && no2 < 70 && pm10 < 50 && pm2_5 < 25 && o3 < 100 && co < 9400){
        air_quality = "Fair";
    } else if (so2 < 250 && no2 < 150 && pm10 < 100 && pm2_5 < 50 && o3 < 140 && co < 12400) {
        air_quality = "Moderate";
    } else if (so2 < 350 && no2 < 200 && pm10 < 200 && pm2_5 < 75 && o3 < 180 && co < 15400) {
        air_quality = "Poor";
    } else {
        air_quality = "Very poor";
    }

    return {air_quality};
}



int main() {
    // https://everything.curl.dev/helpers/url/parse.html
    CURL *curl;
    const std::string api_key = "cdc4242f13de51119450c25f18455def";
    const std::string city = "Ústí+nad+Orlicí"; // Města s mezerama se musí spojovat znaménkem "+" a nesmí být mezera

    curl = curl_easy_init();
    if (curl) {
        auto pair = get_lat_lon(curl, api_key, city);
        if (!pair.has_value()) return 0;
        cout << "lat: " << pair->first << " lon: " << pair->second << endl;

        auto weather = get_weather(curl, api_key, city, pair->first, pair->second);
        if (!weather.has_value()) return 0;
        print_weather(weather.value());

        auto air_quality = get_air_quality(curl, api_key, pair->first, pair->second);
        if (!air_quality.has_value()) return 0;
        cout << air_quality.value() << endl;

        curl_easy_cleanup(curl);    // Free curl v podstatě
    }

    return 0;
}
