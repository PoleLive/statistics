#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TableWidget.h"
#include <QStatusBar>
#include <QPushButton>
#include <QToolBar>
#include <QFileDialog>
#include "analogclock.h"
#include "StatisticsWidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //TableWidget *widget = new TableWidget("TestResult");

    resize(1920, 1080);
    m_pHomeWidget = new AnalogClock(this);
    setCentralWidget(m_pHomeWidget);

    QToolBar *pToolBar = new QToolBar();
    //QPushButton *pButton = new QPushButton("");
    //pButton->setObjectName("OpenDatabase");
    //pStatusBar->addWidget(pButton);
    QAction *pHomeAction = new QAction(QIcon(":/icon/home.png"), "主页");
    QAction *pDatabaseAction = new QAction(QIcon(":/icon/file.png"), "打开数据库");
    QAction *pStatisticsAction = new QAction(QIcon(":/icon/statistics.png"), "统计");
    QAction *pPrintAction = new QAction(QIcon(":/icon/print.png"), "打印预览");
    QAction *pSettingAction = new QAction(QIcon(":/icon/setting.png"), "设置");
    pToolBar->addAction(pHomeAction);
    pToolBar->addAction(pDatabaseAction);
    pToolBar->addAction(pStatisticsAction);
    pToolBar->addAction(pPrintAction);
    pToolBar->addAction(pSettingAction);
    pToolBar->setIconSize(QSize(80, 50));
    connect(pDatabaseAction, &QAction::triggered, [=](){
        QStringList	fileList = QFileDialog::getOpenFileNames(this, "打开数据库", "", "*.db");
        TableWidget *widget = new TableWidget(fileList);
        setCentralWidget(widget);
    });
    connect(pHomeAction, &QAction::triggered, [=](){
        m_pHomeWidget = new AnalogClock(this);
        setCentralWidget(m_pHomeWidget);
    });
    connect(pStatisticsAction, &QAction::triggered, [=](){
       StatisticsWidget *pStatisticsWidget = new StatisticsWidget();
       setCentralWidget(pStatisticsWidget);
    });
    pToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addToolBar(pToolBar);
}


MainWindow::~MainWindow()
{
    delete ui;
}
