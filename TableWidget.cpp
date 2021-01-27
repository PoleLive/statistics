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
#include <QFileInfo>
#include "PrintSettingDialog.h"
#include "ConfigManager.h"
#include "HistogramWidget.h"
#include <QFileInfo>

using namespace std;

TableWidget::TableWidget(const QStringList &strDatabasePaths, QWidget *parent):
    QWidget(parent),
    m_pTree(nullptr)
{
    setWindowTitle("Print Test");
    m_pTableView = new QTableView();
    InitTree(strDatabasePaths);
    m_pLayout = new QGridLayout();
    QPushButton *pPrintButton = new QPushButton("打印");
    QPushButton *pStatisticsButton = new QPushButton("统计");
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
    m_pTree->setMaximumWidth(200);
    m_pTree->setMinimumWidth(200);
    m_pLayout->addWidget(m_pTree, 0, 0, 1, 1);
    m_pLayout->addWidget(m_pTableView, 0, 1, 1, -1);
    m_pLayout->setColumnStretch(1, 1);

    m_pLayout->setRowStretch(0, 1);
    m_pLayout->addWidget(pStatisticsButton, 1, 1, Qt::AlignCenter);
    m_pLayout->addWidget(pPrintButton, 1, 2, Qt::AlignCenter);
    connect(pPrintButton, &QPushButton::clicked, this, &TableWidget::OnPrintSlot);

    //InitStatisticsWidget();
    setLayout(m_pLayout);

    setObjectName("MainContent");
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

void TableWidget::InitTree(const QStringList &strDatabasePaths)
{
    m_pTree = new QTreeWidget(this);
    m_pTree->header()->hide();
    for(int i = 0; i < strDatabasePaths.size(); i++)
    {
        QFileInfo info(strDatabasePaths[i]);
        QTreeWidgetItem *pTopItem = new QTreeWidgetItem();
        pTopItem->setIcon(0, QIcon(":/icon/file.png"));
        pTopItem->setText(0, info.fileName());
        pTopItem->setData(0, Qt::UserRole, "1#"+strDatabasePaths[i]);
        m_pTree->addTopLevelItem(pTopItem);
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(strDatabasePaths[i]);
        if(!db.open())
        {
            QMessageBox::warning(this, "error", db.lastError().text());
        }
        QStringList tableNames = db.tables();
        for(int  j = 0; j < tableNames.size(); j++)
        {
            QTreeWidgetItem *pItem = new QTreeWidgetItem();
            pItem->setText(0, tableNames[j]);
            pItem->setData(0, Qt::UserRole, "2#"+tableNames[j]);
            pItem->setIcon(0, QIcon(":/icon/table2.png"));
            pTopItem->addChild(pItem);
        }
    }
    connect(m_pTree, &QTreeWidget::itemDoubleClicked, [=](QTreeWidgetItem *item, int column){
        QVariant v = item->data(0, Qt::UserRole);
        QStringList infos = v.toString().split("#");
        if(infos.size() < 2)
        {
            return;
        }
        if(infos[0] == "1")
        {

        }
        else
        {
            QTreeWidgetItem *pTopItem = item->parent();
            if(pTopItem != nullptr)
            {
                QVariant vv = pTopItem->data(0, Qt::UserRole);
                m_Database = QSqlDatabase::addDatabase("QSQLITE");
                m_Database.setDatabaseName(vv.toString().split("#").at(1));
                if(!m_Database.open())
                {
                    QMessageBox::warning(this, "error", m_Database.lastError().text());
                }
                m_TableNames = m_Database.tables();

                m_strTableName = infos[1];

                m_pModel = new SqlTableModel(m_pTableView);
                //选择表
                m_pModel->setTable(infos[1]);
                InitTable();
                InitStatisticsWidget();

            }
        }
    });

}

void TableWidget::InitTable()
{
    //把model放在view里面
    m_pTableView->setModel(m_pModel);

    m_pModel->select();

    //只读
    m_pTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //表格宽度随内容调整
    m_pTableView->horizontalHeader();
    m_pTableView->verticalHeader()->hide();
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_pTableView->setContentsMargins(30, 30, 30, 30);

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
    //m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
