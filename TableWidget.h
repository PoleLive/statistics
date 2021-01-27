#pragma once
#include "PrinterManager.h"
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "SqlTableModel.h"

class QGridLayout;
class QTableView;
class HistogramWidget;
class TableWidget : public QWidget
{
    Q_OBJECT
public:
    TableWidget(const QStringList &strDatabasePaths, QWidget *parent = nullptr);
    void InitStatisticsWidget();
    void InitTree(const QStringList &strDatabasePaths);
    void InitTable();
    ~TableWidget();

private slots:
    void OnPrintSlot();

private:
    SqlTableModel           *m_pModel;
    QGridLayout             *m_pLayout;
    QTreeWidget             *m_pTree;
    HistogramWidget         *m_pHistogramWidget;
    QString                 m_strTableName;
    std::map<int, int>      PrintOrderMap;
    std::map<QString, int>  PrintNameOrderMap;
    QList<QString>          PrintFieldList;
    QTableView              *m_pTableView;
    QSqlDatabase            m_Database;
    QStringList             m_TableNames;
};
