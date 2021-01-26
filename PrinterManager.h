#pragma once

#include <QObject>
#include <QAbstractItemModel>
#include <QTextStream>
#include <QPrinter>

class PrinterManager : public QObject
{
    Q_OBJECT
public:
    PrinterManager(QObject *parent = nullptr);

    ~PrinterManager();

    void SetCharSet(const QString &set = QString("utf-8"));

    void DataBegin();

    void DataEnd();

    // 添加主标题（两行：第一行居中显示文字，第二行居右显示时间）
    void InsertTitle(const QString &title);

    // 添加副标题
    void InsertSubtitle(const QString &title);

    // 插入新行
    void InsertNewLine();

    // 不支持嵌套TABLE
    void TableBegin(const QStringList &head);

    void TableEnd();

    void InsertTableRow(const QStringList &RowDatas);

    // 带预览
    void PrintWithPreview();

    // 直接打印，不带预览
    void PrintDirect();

private:
    void ResetState();

private slots:
    void GetPreviewData(QPrinter *printer);

private:
    QString         m_strCharSet;
    QString         m_strData;
    QTextStream     m_TextStream;
};
