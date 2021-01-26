#include "HistogramWidget.h"

#include <QStyleOption>
#include <QPainter>

static const int BOTTOM_HEIGHT = 50;
static const int TOP_HEIGHT = 50;
static const int LEFT_WIDTH = 50;
static const int RIGHT_WIDTH = 50;
static const QColor HISTOGRAM_COLOR1 = QColor(68, 114, 196);
static const QColor HISTOGRAM_COLOR2 = QColor(242, 36, 64);
static const QColor LINE_COLOR = QColor(134, 144, 157);
static const QColor TEXT_COLOR = QColor(0, 0, 0);
static const int DEFAULT_SPACING = 5;
HistogramWidget::HistogramWidget(const QString title,
                                 QList<QString> itemNames,
                                 QList<QString> channelNames,
                                 QList<int> datas1,
                                 QList<int> datas2,
                                 QWidget *parent):
    QWidget(parent),
    m_iSum1(0),
    m_iSum2(0),
    m_ItemNames(itemNames),
    m_ChannelNames(channelNames),
    m_Datas1(datas1),
    m_Datas2(datas2),
    m_strTitle(title)

{

}

void HistogramWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    ComputeMaxNumber();

    DrawTitle(painter);
    DrawBackground(painter);
    DrawHistogram(painter);
    DrawInfo(painter);
    QWidget::paintEvent(event);
}

void HistogramWidget::ComputeMaxNumber()
{
    std::map<std::string, int>::iterator it1;
    std::map<std::string, int>::iterator it2;
    m_iSum1 = m_iSum2 = m_iMaxNumber = 0;
    for(int i = 0; i < m_Datas1.size(); i++)
    {
        if(m_Datas1[i] > m_iMaxNumber)
        {
            m_iMaxNumber = m_Datas1[i];
        }
        m_iSum1 += m_Datas1[i];
    }
    for(int i = 0; i < m_Datas2.size(); i++)
    {
        if(m_Datas2[i] > m_iMaxNumber)
        {
            m_iMaxNumber = m_Datas2[i];
        }
        m_iSum2 += m_Datas2[i];
    }
    m_iMaxNumber++;
    m_iMaxHistogramHeight = height() / 2;
}

QRect HistogramWidget::GetTextRect(const QString text, const QFont &font)
{
    QFontMetrics fm(font);
    QRect rec = fm.boundingRect(text);
    return rec;
}

void HistogramWidget::DrawHistogram(QPainter &painter)
{
    int iDrawNum = m_Datas1.size();

    m_iHistogramWidth = 60;
    double iSpacing = (width() - iDrawNum * m_iHistogramWidth * 2 - LEFT_WIDTH * 4.0 - m_iHistogramWidth * 2) / (iDrawNum - 1);

    double GRAM1_END_X = 0;
    for(int i = 0; i < m_Datas1.size(); i++)
    {
        //绘制柱状图
        if(i % 2 == 0)
        {
            painter.setPen(HISTOGRAM_COLOR1);
            painter.setBrush(HISTOGRAM_COLOR1);
        }
        else
        {
            painter.setPen(HISTOGRAM_COLOR2);
            painter.setBrush(HISTOGRAM_COLOR2);
        }
        double dh = m_iMaxHistogramHeight * (m_Datas1[i]) / (m_iMaxNumber * 1.0);
        double dy = height() - dh - BOTTOM_HEIGHT;
        double dx = GRAM1_END_X = LEFT_WIDTH * 2 + i * m_iHistogramWidth;
        painter.drawRect(dx, dy , m_iHistogramWidth, dh);

        //绘制柱状图上方文字
        painter.setPen(TEXT_COLOR);
        painter.setBrush(TEXT_COLOR);
        QString text = QString::number(m_Datas1[i])+ "(" +QString::number((int)(m_Datas1[i] * 100.0 / m_iSum1  + 0.5))+ "%)";
        QRect rec = GetTextRect(text, font());
        double fx = dx - rec.width() / 2 + m_iHistogramWidth / 2;
        double fy = dy - rec.height() * 2;
        QRectF drawRec(fx, fy, rec.width() + 1, rec.height());
        painter.drawText(drawRec,text);
    }
    {
        QString text = m_ChannelNames[0];
        QRect rec = GetTextRect(text, font());
        double fx = GRAM1_END_X - rec.width() / 2;
        double fy = height() - BOTTOM_HEIGHT + rec.height() * 1.5;
        QRectF drawRec(fx, fy, rec.width() + 4, rec.height());
        painter.drawText(drawRec,text);

    }
    double GRAM2_END_X = 0;
    GRAM1_END_X  += iSpacing + m_iHistogramWidth;

    for(int i = 0; i < m_Datas2.size(); i++)
    {
        //绘制柱状图
        if(i % 2 == 0)
        {
            painter.setPen(HISTOGRAM_COLOR1);
            painter.setBrush(HISTOGRAM_COLOR1);
        }
        else
        {
            painter.setPen(HISTOGRAM_COLOR2);
            painter.setBrush(HISTOGRAM_COLOR2);
        }
        double dh = m_iMaxHistogramHeight * (m_Datas2[i]) / (m_iMaxNumber * 1.0);
        double dy = height() - dh - BOTTOM_HEIGHT;
        double dx = GRAM2_END_X = GRAM1_END_X + LEFT_WIDTH * 2 + i * m_iHistogramWidth;
        painter.drawRect(dx, dy , m_iHistogramWidth, dh);

        //绘制柱状图上方文字
        painter.setPen(TEXT_COLOR);
        painter.setBrush(TEXT_COLOR);
        QString text = QString::number(m_Datas2[i]) + "(" +QString::number((int)(m_Datas2[i]* 100.0 / m_iSum2 + 0.5))+ "%)";
        QRect rec = GetTextRect(text, font());
        double fx = dx - rec.width() / 2 + m_iHistogramWidth / 2 ;
        double fy = dy - rec.height() * 2;
        QRectF drawRec(fx, fy, rec.width() + 1, rec.height());
        painter.drawText(drawRec,text);
    }
    {
        QString text = m_ChannelNames[1];
        QRect rec = GetTextRect(text, font());
        double fx = GRAM2_END_X - rec.width() / 2;
        double fy = height() - BOTTOM_HEIGHT + rec.height() * 1.5;
        QRectF drawRec(fx, fy, rec.width() + 4, rec.height());
        painter.drawText(drawRec,text);

    }
}

void HistogramWidget::DrawTitle(QPainter &painter)
{
    //绘制统计图标题
    QFont font = painter.font();
    font.setPixelSize(30);
    painter.setFont(font);
    QRect rec = GetTextRect(m_strTitle, font);
    double dx = width() / 2 - rec.width() / 2;
    double dy = 60;
    painter.drawText(dx, dy, m_strTitle);
}

void HistogramWidget::DrawBackground(QPainter &painter)
{
    //绘制背景网格
    painter.setFont(font());
    painter.setPen(LINE_COLOR);
    painter.setBrush(LINE_COLOR);

    //横线起始X坐标与结束的X坐标
    double lx1 = LEFT_WIDTH;
    double lx2 = width() - RIGHT_WIDTH;

    int STEP = 1;
    if(m_iMaxNumber > 10)
        STEP = m_iMaxNumber / 10;

    for(int i = 0; i <= m_iMaxNumber; i += STEP)
    {
        int iHistogramHeight = m_iMaxHistogramHeight * i / (m_iMaxNumber * 1.0);;
        double ly = height() - iHistogramHeight - BOTTOM_HEIGHT;
        painter.drawLine(lx1, ly, lx2, ly);

        //绘制横线左侧文字说明（数量）
        QRect rec = GetTextRect(QString::number(i), font());
        double fx = LEFT_WIDTH - rec.width() - DEFAULT_SPACING;
        double fy = height() - iHistogramHeight - BOTTOM_HEIGHT - rec.height() / 2;
        QRectF drawRec(fx, fy, rec.width(), rec.height());
        painter.drawText(drawRec, QString::number(i));
    }
}

void HistogramWidget::DrawInfo(QPainter &painter)
{
    painter.setPen(HISTOGRAM_COLOR1);
    painter.setBrush(HISTOGRAM_COLOR1);
    double dy1 = TOP_HEIGHT * 2;
    double dx1 = LEFT_WIDTH;
    painter.drawRect(dx1, dy1, 60, 15);

    painter.setPen(TEXT_COLOR);
    painter.setBrush(TEXT_COLOR);
    QString text1 = m_ItemNames[0];
    QRect rec1 = GetTextRect(text1, font());
    double fx1 = dx1 + 60 + 10;
    double fy1 = dy1 +(15 - rec1.height()) / 2;
    QRectF drawRec1(fx1, fy1, rec1.width() + 4, rec1.height());
    painter.drawText(drawRec1,text1);

    double dy2 = TOP_HEIGHT * 2 + 30;
    double dx2 = LEFT_WIDTH;
    painter.setPen(HISTOGRAM_COLOR2);
    painter.setBrush(HISTOGRAM_COLOR2);
    painter.drawRect(dx2, dy2, 60, 15);

    painter.setPen(TEXT_COLOR);
    painter.setBrush(TEXT_COLOR);
    QString text2 = m_ItemNames[1];
    QRect rec2 = GetTextRect(text2, font());
    double fx2 = dx2 + 60 + 10;
    double fy2 = dy2 +(15 - rec2.height()) / 2;
    QRectF drawRec2(fx2, fy2, rec2.width() + 4, rec2.height());

    painter.drawText(drawRec2,text2);
}
