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
    cout << "sea level: " << weather.sea_level << endl;
    cout << "ground_level: " << weather.ground_level << endl;
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
    const std::string url = "http://api.openweathermap.org/geo/1.0/direct?q=" + city + "&limit=1&appid=" + api_key;

    auto response = send_request(curl, url);
    if (!response.has_value()) return std::nullopt;

    rapidjson::Document document;
    document.Parse(response.value().c_str());

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

    Weather weather;

    // cout << response.value() << endl;

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
    weather.sea_level = std::to_string(document["main"]["sea_level"].GetInt());
    weather.ground_level = std::to_string(document["main"]["grnd_level"].GetInt());
    weather.visibility = std::to_string(document["visibility"].GetInt());
    weather.wind_speed = std::to_string(document["wind"]["speed"].GetFloat());
    weather.cloudiness = std::to_string(document["clouds"]["all"].GetInt());
    weather.data_time = std::to_string(document["dt"].GetInt());
    weather.sunrise = std::to_string(document["sys"]["sunrise"].GetInt());
    weather.sunset = std::to_string(document["sys"]["sunset"].GetInt());
    weather.timezone = std::to_string(document["timezone"].GetInt() / 3600);

    return {weather};
}

std::optional<AirPollution>
get_pollution(CURL *curl, const std::string &api_key, const std::string &lat, const std::string &lon) {
    // http://api.openweathermap.org/data/2.5/air_pollution/forecast?lat={lat}&lon={lon}&appid={API key}
    const std::string url =
            "http://api.openweathermap.org/data/2.5/air_pollution?lat=" + lat + "&lon=" + lon + "&appid=" +
            api_key;

    cout << send_request(curl, url).value();

    return {AirPollution{}};
}

int main() {
    // https://everything.curl.dev/helpers/url/parse.html
    CURL *curl;
    const std::string api_key = "cdc4242f13de51119450c25f18455def";
    const std::string city = "Olomouc";

    curl = curl_easy_init();
    if (curl) {
        auto pair = get_lat_lon(curl, api_key, city);
        if (!pair.has_value()) return 0;

        auto weather = get_weather(curl, api_key, city, pair->first, pair->second);
        if (!weather.has_value()) return 0;

        // print_weather(weather.value());

        auto pollution = get_pollution(curl, api_key, pair->first, pair->second);
        if (!pollution.has_value()) return 0;

        curl_easy_cleanup(curl);    // Free curl v podstatě
    }

    return 0;
}
