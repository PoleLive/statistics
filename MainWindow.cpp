#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TableWidget.h"
#include <QStatusBar>
#include <QPushButton>
#include <QToolBar>
#include "analogclock.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //TableWidget *widget = new TableWidget("TestResult");

    resize(1920, 1080);
    AnalogClock *clock = new AnalogClock(this);
    setCentralWidget(clock);

    QToolBar *pToolBar = new QToolBar();
    //QPushButton *pButton = new QPushButton("");
    //pButton->setObjectName("OpenDatabase");
    //pStatusBar->addWidget(pButton);
    pToolBar->addAction(new QAction(QIcon(":/icon/home.png"), "主界面"));
    pToolBar->addAction(new QAction(QIcon(":/icon/file.png"), "打开数据库"));
    pToolBar->addAction(new QAction(QIcon(":/icon/statistics.png"), "统计"));
    pToolBar->addAction(new QAction(QIcon(":/icon/print.png"), "打印预览"));
    pToolBar->addAction(new QAction(QIcon(":/icon/setting.png"), "设置"));
    pToolBar->setIconSize(QSize(80, 50));

    pToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    addToolBar(pToolBar);
}


MainWindow::~MainWindow()
{
    delete ui;
}
