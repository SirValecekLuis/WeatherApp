//
// Created by Tobias on 18.05.2024.
//

#ifndef WEATHERAPP_APPLICATION_H
#define WEATHERAPP_APPLICATION_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <iostream>

using std::endl;
using std::cout;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onUpdateButtonClicked();

private:
    QLabel *titleLabel;
    QLabel *tempLabel;
    QLabel *humidityLabel;
    QLabel *sunriseLabel;
    QLabel *sunsetLabel;
    QPushButton *updateButton;
};

#endif //WEATHERAPP_APPLICATION_H
