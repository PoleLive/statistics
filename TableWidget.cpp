#include "TableWidget.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QSqlRecord>
#include <vector>
#include <QTableWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QHeaderView>

#include "PrintSettingDialog.h"
#include "ConfigManager.h"
#include "HistogramWidget.h"
#include <QFileInfo>

using namespace std;

TableWidget::TableWidget(const QString &strTableName, QWidget *parent):
    QWidget(parent),
    m_strTableName(strTableName)
{
    this->setWindowTitle("Print Test");

    //SQLite数据库连接
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("data.db");
    if(!database.open())
    {
        QMessageBox::warning(this, "error", database.lastError().text());
    }
    else
    {
        //QMessageBox::information(this, "数据库", "数据库连接成功！");
    }
    QTableView *m_pTableView = new QTableView();
    m_pModel = new SqlTableModel(m_pTableView);
    //选择表
    m_pModel->setTable(strTableName);

    //把model放在view里面
    m_pTableView->setModel(m_pModel);

    m_pModel->select();
    PrintFieldList = ConfigManager::Instance()->ReadConfig();
    for(int i = 0; i < PrintFieldList.size(); i++)
    {
        for(int j = 0; j < m_pModel->columnCount(); j++)
        {
             if(m_pModel->record(0).fieldName(j).toUpper() == PrintFieldList[i].toUpper())
             {
                 PrintOrderMap[i] = j;
                 PrintNameOrderMap[m_pModel->record(0).fieldName(j).toUpper()] = j;
             }
        }
    }

    //只读
    m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //表格宽度随内容调整
    m_pTableView->horizontalHeader();
    m_pTableView->verticalHeader()->hide();
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pTableView->setContentsMargins(30, 30, 30, 30);

    //m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_pLayout = new QGridLayout();
    QPushButton *pPrintButton = new QPushButton("打印");
    //QPushButton *pSettingButton = new QPushButton("打印设置");
    QPushButton *pStatisticsButton = new QPushButton("统计");

    m_pLayout->addWidget(m_pTableView, 0, 0, 1, -1);
    m_pLayout->setRowStretch(0, 1);
    //m_pLayout->addWidget(pSettingButton, 1, 0, Qt::AlignCenter);
    m_pLayout->addWidget(pStatisticsButton, 1, 0, Qt::AlignCenter);
    m_pLayout->addWidget(pPrintButton, 1, 1, Qt::AlignCenter);
    connect(pPrintButton, &QPushButton::clicked, this, &TableWidget::OnPrintSlot);
    //connect(pSettingButton, &QPushButton::clicked, [=](){PrintSettingDialog dialog; dialog.exec();});

    InitStatisticsWidget();

    connect(pStatisticsButton, &QPushButton::clicked, [=](){
        if(m_pHistogramWidget->isVisible())
        {
            m_pTableView->show();
            m_pHistogramWidget->hide();
            pStatisticsButton->setText("统计");
        }
        else
        {
            m_pTableView->hide();
            m_pHistogramWidget->show();
            pStatisticsButton->setText("列表");
        }
    });

    setLayout(m_pLayout);

    QFileInfo info("config.xml");
    if(!info.exists())
    {
        QList<QString> list;
        for(int j = 0; j < m_pModel->columnCount(); j++)
        {
            list.append(m_pModel->record(0).fieldName(j));
        }
        ConfigManager::Instance()->WriteConfig(list);
    }

    setObjectName("MainContent");
    /*
    设置m_pModel编辑模式为手动提交修改
    m_pModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    */
}

void TableWidget::InitStatisticsWidget()
{
    QSqlTableModel *pModel = new QSqlTableModel(this);

    pModel->setTable(m_strTableName);
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
    int r2 = m_pModel->rowCount() - r1;

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
    int c2 = m_pModel->rowCount() - c1;

    QList<QString> itemNames = {"误码率为0", "误码率不为0"};
    QList<QString> channelsNames = {"通道1", "通道2"};

    //通道1 误码率为0与不为0的次数
    QList<int> data1 = {r1, r2};
    //通道2 误码率为0与不为0的次数
    QList<int> data2 = {c1, c2};

    QString title = "统计项：误码率";

    m_pHistogramWidget = new HistogramWidget(title, itemNames, channelsNames, data1, data2);
    m_pLayout->addWidget(m_pHistogramWidget, 0, 0, 1, -1, Qt::AlignHCenter);
    m_pHistogramWidget->setMinimumWidth(600);
    m_pHistogramWidget->hide();
}

TableWidget::~TableWidget()
{
}
//按下打印按钮
void TableWidget::OnPrintSlot()
{
    int iRowCount = m_pModel->rowCount();
    QString title = QString("Student Information");
    QStringList columnNameList;

    //dataLists为各行数据
    vector<QStringList> dataLists;
    for(int i = 0; i < PrintFieldList.size(); i++)
    {
        if(PrintOrderMap.count(i) == 0)
            continue;
        columnNameList << m_pModel->record(0).fieldName(PrintOrderMap[i]);
    }
    for(int i = 0; i < iRowCount; i++)
    {
        QStringList data;
        for(int j = 0; j < PrintFieldList.size(); j++)
        {
            if(PrintOrderMap.count(j) == 0)
                continue;
            data << m_pModel->record(i).value(PrintOrderMap[j]).toString();
        }
        dataLists.push_back(data);
    }
    PrinterManager PM;
    PM.DataBegin();
    PM.InsertTitle(ConfigManager::Instance()->GetPrintTitle());
    PM.InsertSubtitle(ConfigManager::Instance()->GetPrintSubTitle());
    PM.TableBegin(columnNameList);
    for(int row = 0; row < iRowCount; row++)
    {
        PM.InsertTableRow(dataLists[row]);
    }

    PM.TableEnd();
    PM.DataEnd();
    PM.PrintWithPreview();
}
