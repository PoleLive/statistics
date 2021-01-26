#pragma once

#include <QtXml>
#include <QString>
#include <QList>

class ConfigManager
{
public:
    ConfigManager();
    static ConfigManager *Instance();
    QList<QString> ReadConfig();
    void WriteConfig(QList<QString> fields);
    QString GetPrintTitle();
    QString GetPrintSubTitle();

    QString GetTypeField();
    QString GetTypeValue();
    QString GetDataField();
    QString GetDataValue();
private:
    QString m_strTitle;
    QString m_strSubTitle;
    QString m_strTypeField;
    QString m_strTypeValue;
    QString m_strDataField;
    QString m_strDataValue;
};
