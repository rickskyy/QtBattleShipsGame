#ifndef FLEETCONTROLLER_H
#define FLEETCONTROLLER_H

#include <ctime>
#include <QDebug>

#include "fleet.h"
#include "fieldmodel.h"

class FleetController
{
public:
    FleetController(FieldModel* field);
    
    void setShip(int n, bool hidden); // create and set a ship at random position on a field
    QPair<Ship*, ShotResult> findShip(QPair<int, int> cell);
    FieldModel* createFleet(bool hidden);

    FieldModel *getField() const;
    void setField(FieldModel *value);
    Fleet *getFleet() const;
    void setFleet(Fleet *value);

private:
    FieldModel* field;
    Fleet* fleet;

    void setShipData(QVector<QPair<int, int>> coords, CellType type);
    void createShip(QVector<QPair<int, int>> shipCords, QVector<QPair<int, int>> surroundCords, int size, bool hidden);
    QVector<QPair<int, int>> isolateShip(QPair<int, int> start, QPair<int, int> end);
    
};

#endif // FLEETCONTROLLER_H
