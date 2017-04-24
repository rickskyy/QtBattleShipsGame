#ifndef FLEET_H
#define FLEET_H

#include "ship.h"
#include "constants.h"

class Fleet
{
public:
    Fleet();
    ~Fleet();
    
    QVector<Ship*> vecShips;
    QVector<int> aliveFleetSizes;

    bool getFleetSunk() const;
    void setFleetSunk(bool value);

private:
    bool fleetSunk;
};

#endif // FLEET_H
