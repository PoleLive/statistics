#pragma once

#include <QSqlTableModel>
class SqlTableModel : public QSqlTableModel
{
public:
    SqlTableModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &index, int role) const;
};

