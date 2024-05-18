#include <QApplication>

#include "open_weather.h"
#include "weather_api.h"
#include "application.h"


int main(int argc, char *argv[]) {
    // https://everything.curl.dev/helpers/url/parse.html
    CURL *curl;
    const std::string city = "London"; // Města s mezerama se musí spojovat znaménkem "+" a nesmí být mezera

    curl = curl_easy_init();
    if (curl) {
        auto pair = get_lat_lon(curl, city);
        if (!pair.has_value()) return 0;

        auto weather = get_weather(curl, city, pair->first, pair->second);
        if (!weather.has_value()) return 0;
        print_weather(weather.value());

        // Testování 2. API
        weather = get_weather(curl, city);
        if (!weather.has_value()) return 0;
        print_weather(weather.value());

        auto astronomy = get_astronomy(curl, city);
        if (!astronomy.has_value()) return 0;
        print_astronomy(astronomy.value());

        curl_easy_cleanup(curl);    // Free curl v podstatě
    }

//    QApplication app(argc, argv);
//
//    MainWindow main_window;
//    main_window.show();

    return QApplication::exec();
}
