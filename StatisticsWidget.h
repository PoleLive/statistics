#pragma once
#include <QWidget>
#include <map>
#include <QList>
#include <QStringList>
#include "HistogramWidget.h"
class QSqlTableModel;
class QGridLayout;
class QComboBox;
class StatisticsWidget : public QWidget
{
    Q_OBJECT
public:
    StatisticsWidget(QWidget *parent = nullptr);
    void InitData();
    void InitSelection();
    void InitLayout();
    void CreateWidget();
    void UpdateContent();
    QStringList Parse2Line(const QString &text);
    QStringList Parse(const QString &text);
    QStringList GetData(const QString &text);
private:
    HistogramWidget         *m_pHistogramWidget;
    std::map<int, int>       PrintOrderMap;
    std::map<QString, int>   PrintNameOrderMap;
    QList<QString>           PrintFieldList;
    QSqlTableModel          *m_pModel;
    QGridLayout             *m_pLayout;
    QComboBox *m_pType;
};

