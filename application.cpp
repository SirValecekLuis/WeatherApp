//
// Created by Tobias on 18.05.2024.
//

#include "application.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    // Nastavení okna
    this->setWindowTitle("WeatherApp");
    this->setMinimumSize(400, 300);

    // Vytvoření widgetů
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    titleLabel = new QLabel("Weather Application", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #ffffff;");

    tempLabel = new QLabel("Temperature: --", this);
    humidityLabel = new QLabel("Humidity: --", this);
    sunriseLabel = new QLabel("Sunrise: --", this);
    sunsetLabel = new QLabel("Sunset: --", this);

    updateButton = new QPushButton("Update", this);
    updateButton->setStyleSheet(
            "background-color: #0078d7; color: #ffffff; font-size: 16px; padding: 10px; border-radius: 5px;");
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateButtonClicked);

    // Přidání widgetů do layoutu
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(tempLabel);
    mainLayout->addWidget(humidityLabel);
    mainLayout->addWidget(sunriseLabel);
    mainLayout->addWidget(sunsetLabel);
    mainLayout->addWidget(updateButton);

    // Nastavení layoutu pro centrální widget
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Nastavení stylů
    this->setStyleSheet("QMainWindow { background-color: #2b2b2b; }"
                        "QLabel { font-size: 18px; color: #ffffff; }"
                        "QPushButton { background-color: #0078d7; color: #ffffff; font-size: 16px; padding: 10px; border-radius: 5px; }"
                        "QPushButton:hover { background-color: #005bb5; }");

    // Inicializace
    onUpdateButtonClicked();
}

MainWindow::~MainWindow() {

}

void MainWindow::onUpdateButtonClicked() {
    cout << "cc" << endl;
}

