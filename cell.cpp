#include "cell.h"

Cell::Cell()
{
    state = EMPTY;
    triggered = false;
}

bool Cell::getTriggered() const
{
    return triggered;
}

void Cell::setTriggered(bool value)
{
    triggered = value;
}

CellType Cell::getType() const
{
    return state;
}

void Cell::setType(const CellType &value)
{
    state = value;
}
