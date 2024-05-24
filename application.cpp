#include "application.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setMinimumSize(800, 600);

    setup_UI();
    initialize_data();
    reset_data();
}

MainWindow::~MainWindow() {
    curl_easy_cleanup(curl);
}

void MainWindow::setup_UI() {
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(centralWidget);

    auto *menuBar = new QMenuBar(this);
    auto *apiMenu = new QMenu("API", this);
    auto *openWeatherMapAction = new QAction("OpenWeatherMap", this);
    auto *weatherAPIAction = new QAction("WeatherAPI", this);

    connect(openWeatherMapAction, &QAction::triggered, this, [this]() {
        chosen_api = 1;
        reset_data();
    });

    connect(weatherAPIAction, &QAction::triggered, this, [this]() {
        chosen_api = 2;
        reset_data();
    });

    apiMenu->addAction(openWeatherMapAction);
    apiMenu->addAction(weatherAPIAction);
    menuBar->addMenu(apiMenu);
    setMenuBar(menuBar);

    auto *inputLayout = new QHBoxLayout();
    cityInput = new QLineEdit(this);
    cityInput->setPlaceholderText("Enter city name");
    fetchDataButton = new QPushButton("Fetch Data", this);

    connect(fetchDataButton, &QPushButton::clicked, this, &::MainWindow::reset_data);

    inputLayout->addWidget(cityInput);
    inputLayout->addWidget(fetchDataButton);

    auto *buttonLayout = new QHBoxLayout();
    weatherButton = new QPushButton("Weather", this);
    astronomyButton = new QPushButton("Astronomy", this);

    connect(weatherButton, &QPushButton::clicked, this, &::MainWindow::show_weather);
    connect(astronomyButton, &QPushButton::clicked, this, &::MainWindow::show_astronomy);

    buttonLayout->addWidget(weatherButton);
    buttonLayout->addWidget(astronomyButton);

    stackedWidget = new QStackedWidget(this);
    weatherWidget = new QWidget(this);
    astronomyWidget = new QWidget(this);

    stackedWidget->addWidget(weatherWidget);
    stackedWidget->addWidget(astronomyWidget);

    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(stackedWidget);

    setCentralWidget(centralWidget);
}

QGroupBox *MainWindow::create_weather_group(const QString &title) {
    auto *groupBox = new QGroupBox(title, this);
    auto *layout = new QVBoxLayout();
    groupBox->setLayout(layout);
    return groupBox;
}

QGroupBox *MainWindow::create_astronomy_group(const QString &title) {
    auto *groupBox = new QGroupBox(title, this);
    auto *layout = new QVBoxLayout();
    groupBox->setLayout(layout);
    return groupBox;
}

void MainWindow::initialize_data() {
    auto *weatherLayout = new QVBoxLayout(weatherWidget);

    QGroupBox *locationGroup = create_weather_group("Location");
    weatherCityLabel = new QLabel("City: Not available");
    astronomyLocalTimeLabel = new QLabel("Local Time: Not available");
    locationGroup->layout()->addWidget(weatherCityLabel);
    locationGroup->layout()->addWidget(astronomyLocalTimeLabel);

    QGroupBox *temperatureGroup = create_weather_group("Temperature");
    weatherDescriptionLabel = new QLabel("Description: Not available");
    weatherTempLabel = new QLabel("Temperature: Not available");
    weatherFeelsLikeLabel = new QLabel("Feels Like: Not available");
    temperatureGroup->layout()->addWidget(weatherDescriptionLabel);
    temperatureGroup->layout()->addWidget(weatherTempLabel);
    temperatureGroup->layout()->addWidget(weatherFeelsLikeLabel);

    QGroupBox *detailsGroup = create_weather_group("Details");
    weatherPressureLabel = new QLabel("Pressure: Not available");
    weatherHumidityLabel = new QLabel("Humidity: Not available");
    weatherVisibilityLabel = new QLabel("Visibility: Not available");
    weatherWindSpeedLabel = new QLabel("Wind Speed: Not available");
    weatherCloudinessLabel = new QLabel("Cloudiness: Not available");
    weatherAirQualityLabel = new QLabel("Air Quality: Not available");
    detailsGroup->layout()->addWidget(weatherPressureLabel);
    detailsGroup->layout()->addWidget(weatherHumidityLabel);
    detailsGroup->layout()->addWidget(weatherVisibilityLabel);
    detailsGroup->layout()->addWidget(weatherWindSpeedLabel);
    detailsGroup->layout()->addWidget(weatherCloudinessLabel);
    detailsGroup->layout()->addWidget(weatherAirQualityLabel);

    weatherLayout->addWidget(locationGroup);
    weatherLayout->addWidget(temperatureGroup);
    weatherLayout->addWidget(detailsGroup);

    weatherWidget->setLayout(weatherLayout);

    auto *astronomyLayout = new QVBoxLayout(astronomyWidget);

    QGroupBox *sunGroup = create_astronomy_group("Sun Information");
    astronomySunriseLabel = new QLabel("Sunrise: Not available");
    astronomySunsetLabel = new QLabel("Sunset: Not available");
    sunGroup->layout()->addWidget(astronomySunriseLabel);
    sunGroup->layout()->addWidget(astronomySunsetLabel);

    QGroupBox *moonGroup = create_astronomy_group("Moon Information");
    astronomyMoonriseLabel = new QLabel("Moonrise: Not available");
    astronomyMoonsetLabel = new QLabel("Moonset: Not available");
    astronomyMoonPhaseLabel = new QLabel("Moon Phase: Not available");
    astronomyMoonIlluminationLabel = new QLabel("Moon Illumination: Not available");
    astronomyNextFullMoonLabel = new QLabel("Next Full Moon: Not available");
    moonGroup->layout()->addWidget(astronomyMoonriseLabel);
    moonGroup->layout()->addWidget(astronomyMoonsetLabel);
    moonGroup->layout()->addWidget(astronomyMoonPhaseLabel);
    moonGroup->layout()->addWidget(astronomyMoonIlluminationLabel);
    moonGroup->layout()->addWidget(astronomyNextFullMoonLabel);

    astronomyLayout->addWidget(sunGroup);
    astronomyLayout->addWidget(moonGroup);

    astronomyWidget->setLayout(astronomyLayout);
}

void MainWindow::update_weather_data(const Weather &weather) {
    weatherCityLabel->setText("City: " + QString::fromStdString(weather.name));
    weatherDescriptionLabel->setText("Weather: " + QString::fromStdString(weather.description));
    weatherTempLabel->setText("Temperature: " + QString::fromStdString(weather.temp) + " °C");
    weatherFeelsLikeLabel->setText("Feels Like: " + QString::fromStdString(weather.feels_like) + " °C");
    weatherPressureLabel->setText("Pressure: " + QString::fromStdString(weather.pressure));
    weatherHumidityLabel->setText("Humidity: " + QString::fromStdString(weather.humidity) + "%");
    weatherVisibilityLabel->setText("Visibility: " + QString::fromStdString(weather.visibility) + "%");
    weatherWindSpeedLabel->setText("Wind Speed: " + QString::fromStdString(weather.wind_speed) + " km/h");
    weatherCloudinessLabel->setText("Cloudiness: " + QString::fromStdString(weather.cloudiness) + "%");
    weatherAirQualityLabel->setText(
            "Air Quality (Good, Fair, Moderate, Poor, Very Poor): " + QString::fromStdString(weather.air_quality));
}

void MainWindow::update_astronomy_data(const Astronomy &astronomy) {
    astronomyLocalTimeLabel->setText("Local Time: " + QString::fromStdString(astronomy.local_time));
    astronomySunriseLabel->setText("Sunrise: " + QString::fromStdString(astronomy.sunrise));
    astronomySunsetLabel->setText("Sunset: " + QString::fromStdString(astronomy.sunset));
    astronomyMoonriseLabel->setText("Moonrise: " + QString::fromStdString(astronomy.moonrise));
    astronomyMoonsetLabel->setText("Moonset: " + QString::fromStdString(astronomy.moonset));
    astronomyMoonPhaseLabel->setText("Moon Phase: " + QString::fromStdString(astronomy.moon_phase));
    astronomyMoonIlluminationLabel->setText(
            "Moon Illumination: " + QString::fromStdString(astronomy.moon_illumination) + "%");
    astronomyNextFullMoonLabel->setText("Next Full Moon: " + QString::fromStdString(astronomy.next_full_moon));
}

void MainWindow::show_weather() {
    stackedWidget->setCurrentWidget(weatherWidget);
}

void MainWindow::show_astronomy() {
    stackedWidget->setCurrentWidget(astronomyWidget);
}

void MainWindow::reset_data() {
    QString city = cityInput->text();
    if (city.isEmpty()) {
        return;
    }

    std::string city_text_updated = city.toStdString();
    std::replace(city_text_updated.begin(), city_text_updated.end(), ' ', '+');

    auto weather_data = get_weather_data(city_text_updated);
    auto astronomy_data = get_astronomy_data(city_text_updated);

    if (weather_data.has_value() && !weather_data.value().name.empty()) {

        update_weather_data(weather_data.value());
    } else {
        Weather weather;
        update_weather_data(weather);
    }

    if (astronomy_data.has_value() && weather_data.has_value() && !weather_data.value().name.empty()) {

        update_astronomy_data(astronomy_data.value());
    } else {
        Astronomy astronomy;
        update_astronomy_data(astronomy);
    }
}

std::optional<Weather> MainWindow::get_weather_data(const std::string &city) {
    std::optional<Weather> weather;

    if (curl) {
        // 1. API
        if (chosen_api == 1) {
            auto pair = get_lat_lon(curl, city);
            if (!pair.has_value()) return std::nullopt;

            weather = get_weather(curl, pair->first, pair->second);
            if (!weather.has_value()) return std::nullopt;
        } else if (chosen_api == 2) {
            // 2. API
            weather = get_weather(curl, city);
            if (!weather.has_value()) return std::nullopt;
        }
    }

    return {weather};
}

std::optional<Astronomy> MainWindow::get_astronomy_data(const std::string &city) {
    std::optional<Astronomy> astronomy;

    if (curl) {
        astronomy = get_astronomy(curl, city);
        if (!astronomy.has_value()) return std::nullopt;
    }

    return {astronomy};
}