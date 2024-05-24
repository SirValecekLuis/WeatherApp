#ifndef WEATHERAPP_APPLICATION_H
#define WEATHERAPP_APPLICATION_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QtWidgets>

#include "open_weather.h"
#include "weather_api.h"
#include "application.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void update_weather_data(const Weather &weather);
    void update_astronomy_data(const Astronomy &astronomy);

private slots:
    void show_weather();
    void show_astronomy();
    void reset_data();

private:
    void setup_UI();
    void initialize_data();
    QGroupBox* create_weather_group(const QString &title);
    QGroupBox* create_astronomy_group(const QString &title);
    std::optional<Weather> get_weather_data(const std::string &city);
    std::optional<Astronomy> get_astronomy_data(const std::string &city);

    CURL* curl = curl_easy_init();

    QStackedWidget *stackedWidget{};
    QWidget *weatherWidget{};
    QWidget *astronomyWidget{};
    QPushButton *weatherButton{};
    QPushButton *astronomyButton{};

    QLineEdit *cityInput{};
    QPushButton *fetchDataButton{};

    QLabel *weatherCityLabel{};
    QLabel *weatherDescriptionLabel{};
    QLabel *weatherTempLabel{};
    QLabel *weatherFeelsLikeLabel{};
    QLabel *weatherPressureLabel{};
    QLabel *weatherHumidityLabel{};
    QLabel *weatherVisibilityLabel{};
    QLabel *weatherWindSpeedLabel{};
    QLabel *weatherCloudinessLabel{};
    QLabel *weatherAirQualityLabel{};

    QLabel *astronomyLocalTimeLabel{};
    QLabel *astronomySunriseLabel{};
    QLabel *astronomySunsetLabel{};
    QLabel *astronomyMoonriseLabel{};
    QLabel *astronomyMoonsetLabel{};
    QLabel *astronomyMoonPhaseLabel{};
    QLabel *astronomyMoonIlluminationLabel{};
    QLabel *astronomyNextFullMoonLabel{};

    int chosen_api = 1;
};

#endif //WEATHERAPP_APPLICATION_H
