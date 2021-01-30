#pragma once

#include <QWidget>
#include <map>
#include <string>
#include <QList>
#include <QString>

class HistogramWidget : public QWidget
{
    Q_OBJECT
public:
    HistogramWidget(const QString title, QList<QString> itemNames,
                    QList<QString> channelNames ,QList<int> datas1,
                    QList<int> datas2,QWidget *parent = nullptr);
    void SetData(const QString title, QList<QString> itemNames,
                 QList<QString> channelNames ,QList<int> datas1,
                 QList<int> datas2);
    void paintEvent(QPaintEvent *event) override;
private:
    void ComputeMaxNumber();
    void DrawHistogram(QPainter &painter);
    void DrawTitle(QPainter &painter);
    void DrawBackground(QPainter &painter);
    void DrawInfo(QPainter &painter);
    QRect GetTextRect(const QString text, const QFont &font);

private:
    QList<int> m_Datas1;
    QList<int> m_Datas2;
    QList<QString> m_ItemNames;
    QList<QString> m_ChannelNames;
    int m_iMaxNumber;
    QString m_strTitle;
    double m_iMaxHistogramHeight;
    double m_iHistogramWidth;
    int m_iSum1;
    int m_iSum2;
};
