#include "fieldmodel.h"
#include <typeinfo>
#include <QDebug>
#include <iostream>

FieldModel::FieldModel(int rows, int columns)
{
    selectable = true;
    this->rows = rows;
    this->columns = columns;
    generateField(rows, columns);
}

void FieldModel::generateField(int rows, int columns) {
    /*
     * Creates field and stores Cell class instances value
     */
    for(int row = 0; row < rows; row++)
        {
            for(int col = 0; col < columns; col++)
            {
                QStandardItem *item = new QStandardItem();
                Cell* c = new Cell();
                item->setData(QVariant::fromValue(c));
                item->setBackground(QBrush(Qt::darkCyan, Qt::SolidPattern));
                this->setItem(row, col, item);
            }
        }
}

int FieldModel::rowCount(const QModelIndex &) const
{
    return this->rows;
}

int FieldModel::columnCount(const QModelIndex &) const
{
    return this->columns;
}

Qt::ItemFlags FieldModel::flags(const QModelIndex &index) const
{
    if (selectable)
        return Qt::ItemIsEnabled;
    else
        return Qt::NoItemFlags;
}

