#ifndef SHIP_H
#define SHIP_H

#include <QVector>

class Ship
{
public:
    Ship();
    Ship(QVector<QPair<int, int>> shipCoordinates, QVector<QPair<int, int>> surroundCellCordinates, int size, bool hidden);
    ~Ship();
    
    // vector holds pairs of <row, col> coordinates of a ship
    QVector<QPair<int, int>> shipCoordinates;
    // vector holds pairs of <row, col> coordinates of surrounding cells.
    QVector<QPair<int, int>> surroundCellCordinates;
    
    bool getDead() const;
    void setDead(bool value);

    bool getHidden() const;
    void setHidden(bool value);
    
    int getLiveNumCells() const;
    void setLiveNumCells(int value);
   
    int getDefaultShipSize() const;
    void setDefaultShipSize(int value);
    
private:    
    bool dead;
    bool hidden;
    int liveNumCells;
    int defaultShipSize;
};

#endif // SHIP_H
