#include "fleetcontroller.h"

FleetController::FleetController(FieldModel* field)
{
    this->field = field;
    fleet = new Fleet();
}

FieldModel* FleetController::createFleet(bool hidden)
{
    for (int i = 0; i < FLEET_SIZE; i++) {
        setShip(SHIP_SIZES[i], hidden);
    }
    return field;
}

void FleetController::createShip(QVector<QPair<int, int> > shipCords, QVector<QPair<int, int> > surroundCords,
                                 int size, bool hidden)
{
    Ship* ship = new Ship(shipCords, surroundCords, size, hidden);
    fleet->aliveFleetSizes.push_back(size);
    fleet->vecShips.push_back(ship);
}

void FleetController::setShipData(QVector<QPair<int, int>> coords, CellType type) {
    for (int i = 0; i < coords.size(); i++) {
        field->item(coords[i].first, coords[i].second)->data().value<Cell*>()->setType(type);
    }
}

QVector<QPair<int, int>> FleetController::isolateShip(QPair<int, int> start, QPair<int, int> end)
{
    /*
     * returns vector of surrounding cells
     */
    QVector<QPair<int, int>> result;
    for (int i = start.first; i <= end.first; i++) {
        for (int j = start.second; j <= end.second; j++) {
            if (i >= 0 && i < N && j >= 0 && j < N) {
                switch(field->item(i, j)->data().value<Cell*>()->getType()) {
                    case SHIP:
                        break;
                    case SURROUND:
                        result.push_back(QPair<int,int>(i,j));
                        break;
                    case EMPTY:
                        field->item(i, j)->data().value<Cell*>()->setType(SURROUND);
                        result.push_back(QPair<int,int>(i,j));
                        break;
                } 
            }
        }
    }
    
    return result;
}

void FleetController::setShip(int n, bool hidden) {
    /*
     * Finds possible place on the field to set a ship and creates a ship with the given coordinates.
     */
    bool set = false;
    QVector<QPair<int, int>> tempShip;
    QVector<QPair<int, int>> tempSurroundCells;

    srand(time(NULL));
    while (true){
        int i = rand() % ROWS;
        int j = rand() % COLS;

        if (field->item(i, j)->data().value<Cell*>()->getType() == EMPTY) {
            if (n == 1) {
                tempShip.clear();
                tempSurroundCells.clear();
                
                tempShip.push_back(QPair<int, int>(i, j));
                setShipData(tempShip, SHIP);
                tempSurroundCells = isolateShip(QPair<int, int>(i-1, j-1), QPair<int, int>(i+1, j+1));
                createShip(tempShip, tempSurroundCells, n, hidden);

                break;
            }
            
            std::vector<Direction> dirs{ LEFT, UP, RIGHT, DOWN };
            while (!dirs.empty()) {
                
                 // check if there are possible ways from the certain cell
                int index = rand() % dirs.size();
                std::vector<Direction>::iterator it = dirs.begin() + index;
                Direction way = *it;
                dirs.erase(it);

                switch (way) {
                    case LEFT:
                        for (int k = 1; j - k >= 0 && k < n; k++)
                        {
                            if (field->item(i, j-k)->data().value<Cell*>()->getType() != EMPTY)
                                break;
                            if (k == n - 1)
                                set = true;
                        }
                        if (set)
                        {
                            tempShip.clear();
                            tempSurroundCells.clear();
                            
                            for (int k = 0; k < n; k++)
                            {
                                tempShip.push_back(QPair<int, int>(i, j-k));
                            }
                            setShipData(tempShip, SHIP);
                            tempSurroundCells = isolateShip(QPair<int, int>(i-1, j-n), QPair<int, int>(i+1, j+1));
                            createShip(tempShip, tempSurroundCells, n, hidden);
                        }
                        break;
                    case UP:
                        for (int k = 1; i - k >= 0 && k < n; k++)
                        {
                            if (field->item(i-k, j)->data().value<Cell*>()->getType() != EMPTY)
                                break;
                            if (k == n - 1)
                                set = true;
                        }
                        if (set)
                        {
                            tempShip.clear();
                            tempSurroundCells.clear();

                            for (int k = 0; k < n; k++)
                            {
                                tempShip.push_back(QPair<int, int>(i-k, j));
                            }
                            setShipData(tempShip, SHIP);
                            tempSurroundCells = isolateShip(QPair<int, int>(i-n, j-1), QPair<int, int>(i+1, j+1));
                            createShip(tempShip, tempSurroundCells, n, hidden);
                        }
                        break;
                    case RIGHT:
                        for (int k = 1; j + k < N && k < n; k++)
                        {
                            if (field->item(i, j+k)->data().value<Cell*>()->getType() != EMPTY)
                                break;
                            if (k == n - 1)
                                set = true;
                        }
                        if (set)
                        {
                            tempShip.clear();
                            tempSurroundCells.clear();

                            for (int k = 0; k < n; k++)
                            {
                                tempShip.push_back(QPair<int, int>(i, j+k));
                            }
                            setShipData(tempShip, SHIP);
                            tempSurroundCells = isolateShip(QPair<int, int>(i-1, j-1), QPair<int, int>(i+1, j+n));
                            createShip(tempShip, tempSurroundCells, n, hidden);
                        }
                        break;
                    case DOWN:
                        for (int k = 1; i + k < N && k < n; k++)
                        {
                            if (field->item(i+k, j)->data().value<Cell*>()->getType() != EMPTY)
                                break;
                            if (k == n - 1)
                                set = true;
                        }
                        if (set)
                        {
                            tempShip.clear();
                            tempSurroundCells.clear();

                            for (int k = 0; k < n; k++)
                            {
                                tempShip.push_back(QPair<int, int>(i+k, j));
                            }
                            setShipData(tempShip, SHIP);
                            tempSurroundCells = isolateShip(QPair<int, int>(i-1, j-1), QPair<int, int>(i+n, j+1));
                            createShip(tempShip, tempSurroundCells, n, hidden);
                        }
                        break;
                    }
                    if (set) {
                        dirs.clear();
                        break;
                    }
            }  
        }
        if (set)
            break;
    }
}

QPair<Ship*, ShotResult> FleetController::findShip(QPair<int, int> cell)
{
    /*
     * Find the ship in the fleet's vector of ships. Removes it.
     * Return the ship data if found and result (dead, alive).
     */
    Ship* ship;

    foreach (ship, fleet->vecShips) {
        for (int i = 0; i < ship->shipCoordinates.size(); i++) {
            if (cell == ship->shipCoordinates[i]) {
                ship->shipCoordinates.removeOne(cell);
                ship->setLiveNumCells(ship->getLiveNumCells() - 1);
                if (ship->getLiveNumCells() == 0) {
                    ship->setDead(true);
                    fleet->aliveFleetSizes.removeOne(ship->getDefaultShipSize());

                    if (fleet->aliveFleetSizes.size() == 0)
                        fleet->setFleetSunk(true);
                    return QPair<Ship*, ShotResult>(ship, KILL);
                } else {
                    return QPair<Ship*, ShotResult>(ship, SHOT);
                }
            }
        }
    }
    return QPair<Ship*, ShotResult>(nullptr, MISS);
}

FieldModel *FleetController::getField() const
{
    return field;
}

void FleetController::setField(FieldModel *value)
{
    field = value;
}

Fleet *FleetController::getFleet() const
{
    return fleet;
}

void FleetController::setFleet(Fleet *value)
{
    fleet = value;
}


