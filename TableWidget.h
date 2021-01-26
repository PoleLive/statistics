#pragma once
#include "PrinterManager.h"
#include <QWidget>
#include "SqlTableModel.h"

class QGridLayout;
class HistogramWidget;
class TableWidget : public QWidget
{
    Q_OBJECT
public:
    TableWidget(const QString &strTableName, QWidget *parent = nullptr);
    void InitStatisticsWidget();
    ~TableWidget();

private slots:
    void OnPrintSlot();

private:
    SqlTableModel *m_pModel;
    QGridLayout *m_pLayout;
    HistogramWidget *m_pHistogramWidget;
    QString m_strTableName;
    std::map<int, int> PrintOrderMap;
    std::map<QString, int> PrintNameOrderMap;
    QList<QString> PrintFieldList;
};
