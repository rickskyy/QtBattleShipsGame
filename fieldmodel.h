#ifndef FIELDMODEL_H
#define FIELDMODEL_H
#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QString>

#include "cell.h"

class FieldModel: public QStandardItemModel
{
    Q_OBJECT
public:
    FieldModel(int rows, int columns);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex & index) const Q_DECL_OVERRIDE;

    void generateField(int rows, int columns);

private:
   int rows;
   int columns;
   bool selectable;
};

#endif // FIELDMODEL_H
