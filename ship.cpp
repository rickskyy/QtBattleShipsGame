#include "ship.h"

Ship::Ship()
{
    dead = false;
    liveNumCells = 0;
    defaultShipSize = 0;
    hidden = false;
}

Ship::Ship(QVector<QPair<int, int>> shipCoordinates, QVector<QPair<int, int>> surroundCellCordinates, int size, bool hidden) {
    dead = false;
    liveNumCells = size;
    defaultShipSize = size;
    this->hidden = hidden;
    
    this->shipCoordinates = shipCoordinates;
    this->surroundCellCordinates = surroundCellCordinates;
}

bool Ship::getDead() const
{
    return dead;
}

void Ship::setDead(bool value)
{
    dead = value;
}

bool Ship::getHidden() const
{
    return hidden;
}

void Ship::setHidden(bool value)
{
    hidden = value;
}

int Ship::getLiveNumCells() const
{
    return liveNumCells;
}

void Ship::setLiveNumCells(int value)
{
    liveNumCells = value;
}

int Ship::getDefaultShipSize() const
{
    return defaultShipSize;
}

void Ship::setDefaultShipSize(int value)
{
    defaultShipSize = value;
}
