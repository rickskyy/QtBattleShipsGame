#ifndef CELL_H
#define CELL_H
#include <QStandardItem>

#include "constants.h"

class Cell
{
public:
    Cell();
    
    bool getTriggered() const;
    void setTriggered(bool value);

    CellType getType() const;
    void setType(const CellType &value);

private:
    CellType state;
    bool triggered;

};

Q_DECLARE_METATYPE(Cell*)
#endif // CELL_H
