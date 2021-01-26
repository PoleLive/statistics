#include "SqlTableModel.h"


SqlTableModel::SqlTableModel(QObject *parent, QSqlDatabase db):
    QSqlTableModel(parent, db)
{

}

QVariant SqlTableModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if(role ==  Qt::TextAlignmentRole )
    {
        value   =   (Qt::AlignCenter);
        return value;
    }
    return value;
}
