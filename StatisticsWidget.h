#pragma once
#include <QWidget>
#include <map>
#include <QList>
#include "HistogramWidget.h"
class StatisticsWidget : public QWidget
{
    Q_OBJECT
public:
    StatisticsWidget(QWidget *parent = nullptr);
private:
    HistogramWidget *m_pHistogramWidget;
    std::map<int, int>      PrintOrderMap;
    std::map<QString, int>  PrintNameOrderMap;
    QList<QString>          PrintFieldList;
};

