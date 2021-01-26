#pragma once

#include <QDialog>
class QGroupBox;
class QSqlTableModel;
class PrintSettingDialog : public QDialog
{
    Q_OBJECT
public:
    PrintSettingDialog(QWidget *parent = nullptr);
    void InitLayout();
    void InitTitle();
    void InitSubtitle();
    void InitTable();
    void InitData();
private:
    QGroupBox *m_pTitleBox;
    QGroupBox *m_pSubtitleBox;
    QSqlTableModel *m_pModel;
};

