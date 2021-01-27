#include "StatisticsWidget.h"
#include "ConfigManager.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QGridLayout>
#include <QMessageBox>
#include <QSqlError>
StatisticsWidget::StatisticsWidget(QWidget *parent):
    QWidget(parent),
    m_pHistogramWidget(nullptr)
{


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");
    if(!db.open())
    {
        QMessageBox::warning(this, "error", db.lastError().text());
    }
    QStringList list = db.tables();
    QSqlTableModel *pModel = new QSqlTableModel(this);
    QSqlTableModel *pModel2 = new QSqlTableModel(this);
    pModel->setTable("TestResult");
    pModel2->setTable("TestResult");
    pModel2->select();

    PrintFieldList = ConfigManager::Instance()->ReadConfig();
    for(int i = 0; i < PrintFieldList.size(); i++)
    {
        for(int j = 0; j < pModel2->columnCount(); j++)
        {
            if(pModel2->record(0).fieldName(j).toUpper() == PrintFieldList[i].toUpper())
            {
                PrintOrderMap[i] = j;
                PrintNameOrderMap[pModel2->record(0).fieldName(j).toUpper()] = j;
            }
        }
    }
    QFileInfo info("config.xml");
    if(!info.exists())
    {
        QList<QString> list;
        for(int j = 0; j < pModel2->columnCount(); j++)
        {
            list.append(pModel2->record(0).fieldName(j));
        }
        ConfigManager::Instance()->WriteConfig(list);
    }
    QString filter = ConfigManager::Instance()->GetTypeField() + " = '" +ConfigManager::Instance()->GetTypeValue()+"'";
    pModel->setFilter(filter);
    pModel->select();
    int r1 = 0;
    for(int i = 0; i < pModel->rowCount(); i++)
    {
        if(pModel->record(i).value(PrintNameOrderMap[ConfigManager::Instance()->GetDataField().toUpper()]) == ConfigManager::Instance()->GetDataValue())
        {
            r1++;
        }
    }
    int r2 = pModel->rowCount() - r1;

    pModel->setFilter(ConfigManager::Instance()->GetTypeField() + " != '" +ConfigManager::Instance()->GetTypeValue()+"'");
    pModel->select();

    int c1 = 0;
    for(int i = 0; i < pModel->rowCount(); i++)
    {
        if(pModel->record(i).value(PrintNameOrderMap[ConfigManager::Instance()->GetDataField().toUpper()]) == ConfigManager::Instance()->GetDataValue())
        {
            c1++;
        }
    }
    int c2 = pModel->rowCount() - c1;

    QList<QString> itemNames = {"误码率为0", "误码率不为0"};
    QList<QString> channelsNames = {"通道1", "通道2"};

    //通道1 误码率为0与不为0的次数
    QList<int> data1 = {r1, r2};
    //通道2 误码率为0与不为0的次数
    QList<int> data2 = {c1, c2};

    QString title = "统计项：误码率";

    m_pHistogramWidget = new HistogramWidget(title, itemNames, channelsNames, data1, data2);

    QGridLayout *pLayout = new QGridLayout();
    pLayout->addWidget(m_pHistogramWidget);
    setLayout(pLayout);
}
