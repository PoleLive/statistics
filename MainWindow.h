#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class AnalogClock;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    AnalogClock *m_pHomeWidget;
};


