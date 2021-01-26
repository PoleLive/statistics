#include "PrinterManager.h"

#include <QTextEdit>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QDateTime>

PrinterManager::PrinterManager(QObject *parent) : QObject(parent), m_strCharSet("utf-8")
{
    m_TextStream.setString(&m_strData);
}

PrinterManager::~PrinterManager()
{

}

void PrinterManager::SetCharSet(const QString &set)
{
    m_strCharSet = set;
}

void PrinterManager::DataBegin()
{
    m_TextStream << QString("<html>")
        << QString("<head>")
        << QString("<meta content=\"text/html; charset=%1\">").arg(m_strCharSet)
        << QString("<meta name=Generator content=\"Microsoft Word 12 (filtered)\">")
        << QString("</head>");
}

void PrinterManager::DataEnd()
{
    m_TextStream << QString("</html>");
}

void PrinterManager::InsertTitle(const QString &title)
{
    m_TextStream << QString("<p style='text-align:center'><span style='font-size:20.0pt;font-family:宋体'>%1</span></p>\n")
           .arg(title)
        << QString("<p style='text-align:right'><span style='font-size:18.0pt;font-family:宋体'>打印时间：%1</span></p>\n")
           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
}

void PrinterManager::InsertSubtitle(const QString &title)
{
    m_TextStream << QString("<p style='text-align:right'><span style='font-size:16.0pt;font-family:宋体'><b>-%1</b></span></p>\n")
           .arg(title);
}

void PrinterManager::InsertNewLine()
{
    m_TextStream << QString("<br>\n");
}

void PrinterManager::TableBegin(const QStringList &head)
{
    m_TextStream << QString("<div align='center' style='width:650px;margin-left:-70px;margin-top:-100px'>")
        << QString("<table class=print width='100%' border=1 cellspacing=0 cellpadding=4 style=border-collapse:collapse>\n")
        << QString("<thead><tr>\n");
    for (int i = 0; i < head.count(); ++i)
    {
        m_TextStream << QString("<td nowrap='nowarp' style='border:solid windowtext 2.0pt'><p align='center'><span align='center' style='font-size:16.0pt;font-family:宋体'><b>%1</b></span></p></td>\n")
               .arg(head.at(i));
    }
    m_TextStream << "</tr></thead>\n";
}

void PrinterManager::TableEnd()
{
    m_TextStream << QString("</table></div>\n");
}

void PrinterManager::InsertTableRow(const QStringList &RowDatas)
{
    m_TextStream << QString("<tr>\n");
    for (int i = 0; i < RowDatas.count(); ++i)
    {
        m_TextStream << QString("<td><p><span style='font-size:16.0pt;font-family:宋体'>%1</span></p></td>\n")
               .arg(RowDatas.at(i));
    }
    m_TextStream << QString("</tr>\n");
}

void PrinterManager::PrintWithPreview()
{
    QPrinter printer(QPrinter::ScreenResolution);
    //由于一列的数据项数过多，将打印预览页面设置为水平
    printer.setOrientation(QPrinter::Landscape);
    //设置页边距
    printer.setPageMargins(2, 2, 6, 6, QPrinter::Millimeter);
    printer.setPageSize(QPrinter::A4);
    QPrintPreviewDialog preview(&printer);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(GetPreviewData(QPrinter*)));
    preview.setWindowState(Qt::WindowMaximized);
    preview.exec();
    ResetState();
}

void PrinterManager::PrintDirect()
{
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPageSize(QPrinter::A4);
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted)
    {
        GetPreviewData(&printer);
    }
    ResetState();
}

void PrinterManager::ResetState()
{
    m_strData.clear();
    m_strCharSet = "utf-8";
}

void PrinterManager::GetPreviewData(QPrinter *printer)
{
    QTextDocument document;
    document.setHtml(m_strData);
    document.print(printer);
}
