#include "StatisticsWidget.h"
#include "ConfigManager.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QGridLayout>
#include <QMessageBox>
#include <QComboBox>
#include <QLabel>
#include <QSqlError>
#include <QtDebug>
#include <QString>

std::map<int, int> SelectionMap = {
    {0, 0},
    {1, 2},
    {2, 4},
    {3, 6},
    {4, 0},
    {5, 2},
    {6, 4},
    {7, 6}
};
std::map<int, QString> DefaultValueMap = {
    {0, "0"},
    {1, "成功"},
    {2, "成功"},
    {3, "成功"},
    {4, "0"},
    {5, "成功"},
    {6, "成功"},
    {7, "成功"}
};

std::map<int, QString> TitleMap = {
    {0, "A误码率"},
    {1, "A收钟"},
    {2, "A收码"},
    {3, "A帧同步"},
    {4, "B误码率"},
    {5, "B收钟"},
    {6, "B收码"},
    {7, "B帧同步"}
};
StatisticsWidget::StatisticsWidget(QWidget *parent):
    QWidget(parent),
    m_pHistogramWidget(nullptr)
{
    InitData();
    CreateWidget();
    InitSelection();

    UpdateContent();
    InitLayout();



    QString text= "测试说明：\n\n"
            "起始事件：2019-11-14\n\n"
            "配置单位： 设备类型\n"
            "设备编号：设备编号设备编号设备编号设备编号\n"
            "接口类型：五\n"
            "误码率阈值：无\n\n"
            "误码率测试结果：\n\n"
            "测试时间(s) 19\n"
            "1通道测试码数（bit)  0 2通道测试码数（bit）0\n"
            "1通道误码数 0 2通道误码数 0\n"
            "1通道误码率(%) nan 2通道误码率(%) nan \n"
            "1通道收钟 失败 2通道收钟 失败\n"
            "1通道收码 失败 2通道收码 成功\n"
            "1通道帧同步 失败 2通道帧同步 成功\n"
            "1通道误码率告警 无 2通道误码率告警 无";

}

void StatisticsWidget::InitData()
{
    m_pModel = new QSqlTableModel(this);
    m_pModel->setTable("devTestLog");
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
}

void StatisticsWidget::InitSelection()
{
    QLabel *pTypeLabel = new QLabel("统计类型:");
    QHBoxLayout *pTitleLayout = new QHBoxLayout();
    m_pType = new QComboBox();
    pTitleLayout->addWidget(pTypeLabel);
    pTitleLayout->addWidget(m_pType);
    m_pType->setStyleSheet("font-size: 20pt");
    pTypeLabel->setStyleSheet("font-size: 20pt");
    m_pType->addItem("A误码率统计");
    m_pType->addItem("A收钟统计");
    m_pType->addItem("A收码统计");
    m_pType->addItem("A帧同步统计");

    m_pType->addItem("B误码率统计");
    m_pType->addItem("B收钟统计");
    m_pType->addItem("B收码统计");
    m_pType->addItem("B帧同步统计");

    m_pLayout->addLayout(pTitleLayout, 0, 0, Qt::AlignCenter);
}

void StatisticsWidget::InitLayout()
{
    m_pLayout->addWidget(m_pHistogramWidget, 1, 0, Qt::AlignHCenter);
    m_pLayout->setRowStretch(0, 1);
    m_pLayout->setRowStretch(1, 6);
    setLayout(m_pLayout);
    connect(m_pType, &QComboBox::currentTextChanged, [=](){
        UpdateContent();
    });
}

void StatisticsWidget::CreateWidget()
{
    m_pLayout = new QGridLayout();
}

void StatisticsWidget::UpdateContent()
{
    int iSelectIndex = m_pType->currentIndex();
    QString typeName;
    if(iSelectIndex < 4)
        typeName = "A";
    else
        typeName = "B";
    QString filter = "dev_type = '"+typeName+"'";
    m_pModel->setFilter(filter);
    m_pModel->select();
    int iMsgIndex = 0;

    for(int i = 0; i < m_pModel->columnCount(); i++)
    {
        if(m_pModel->record(0).fieldName(i).toUpper() == "MSG")
        {
            iMsgIndex = i;
        }
    }
    int r1 = 0;
    int c1 = 0;
    for(int i = 0; i < m_pModel->rowCount(); i++)
    {
        QString text = m_pModel->record(i).value(iMsgIndex).toString();
        QStringList res = GetData(text);
        if(res.at(SelectionMap[iSelectIndex]) == DefaultValueMap[iSelectIndex])
            r1++;
        if(res.at(SelectionMap[iSelectIndex] + 1) == DefaultValueMap[iSelectIndex])
            c1++;
    }
    int r2 = m_pModel->rowCount() - r1;
    int c2 = m_pModel->rowCount() - c1;


    QList<QString> itemNames = {TitleMap[iSelectIndex] +"为"+ DefaultValueMap[iSelectIndex],
                                TitleMap[iSelectIndex] +"不为"+ DefaultValueMap[iSelectIndex]};
    QList<QString> channelsNames = {"通道1", "通道2"};

    //通道1 误码率为0与不为0的次数
    QList<int> data1 = {r1, r2};
    //通道2 误码率为0与不为0的次数
    QList<int> data2 = {c1, c2};

    QString title = "";

    if(m_pHistogramWidget == nullptr)
    {
        m_pHistogramWidget = new HistogramWidget(title, itemNames, channelsNames, data1, data2);
    }
    else
    {
        m_pHistogramWidget->SetData(title, itemNames, channelsNames, data1, data2);
    }
    m_pHistogramWidget->setMinimumWidth(600);
}

QStringList StatisticsWidget::Parse2Line(const QString &text)
{
    return text.split("\n");
}

QStringList StatisticsWidget::Parse(const QString &text)
{
    return text.split(" ");
}

QStringList StatisticsWidget::GetData(const QString &text)
{
    QStringList list = Parse2Line(text);
    QStringList lines;
    for(int i = 0; i < list.size(); i++)
    {
        if(list.at(i).contains("1通道"))
        {
            lines.append(list[i]);
        }
    }
    QStringList result;
    for(int i = 2; i < lines.size(); i++)
    {
        QStringList datas = Parse(lines[i]);
        result.append(datas[1]);
        result.append(datas[3]);
    }
    return result;
}


