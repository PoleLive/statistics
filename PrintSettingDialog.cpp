#include "PrintSettingDialog.h"

#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlError>
#include <QStringList>
#include <QSqlRecord>
PrintSettingDialog::PrintSettingDialog(QWidget *parent)
{
    InitTitle();
    InitData();
    InitSubtitle();
    InitTable();
    InitLayout();

}

void PrintSettingDialog::InitLayout()
{
    setWindowTitle("打印设置");
    QGridLayout *pLayout = new QGridLayout();
    pLayout->addWidget(m_pTitleBox, 0, 0);
    pLayout->addWidget(m_pSubtitleBox, 0, 1);
    setLayout(pLayout);
}

void PrintSettingDialog::InitTitle()
{
    m_pTitleBox = new QGroupBox("打印标题");
    QGridLayout *pBoxLayout = new QGridLayout();
    m_pTitleBox->setLayout(pBoxLayout);
    pBoxLayout->addWidget(new QLineEdit());
}

void PrintSettingDialog::InitSubtitle()
{
    m_pSubtitleBox = new QGroupBox("副标题");
    QGridLayout *pBoxLayout = new QGridLayout();
    m_pSubtitleBox->setLayout(pBoxLayout);
    pBoxLayout->addWidget(new QLineEdit());
}

void PrintSettingDialog::InitTable()
{

}

void PrintSettingDialog::InitData()
{
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
    m_pModel = new QSqlTableModel();
    //选择表
    m_pModel->setTable("TestResult");

    m_pModel->select();

}
