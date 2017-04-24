#ifndef SHOTAI_H
#define SHOTAI_H

#include <QPair>
#include <QVector>
#include <QDebug>
#include <algorithm>
#include <ctime>

#include "constants.h"
#include "ship.h"

enum AttackCell { TRIGGERED, UNTRIGGERED };

class ComboShots {
public:
    ComboShots(QPair<int, int> firstHit, QVector<QVector<AttackCell> >& field);

    // stores coordinates of the last successful shots before destroying a ship
    QVector<QPair<int, int>> lastShotsPositions;
    QVector<Direction> possibleDirections;
    Direction previousDirection;
    bool foundDirection = false;

    void findPossibleDirections(QPair<int, int> cell, QVector<QVector<AttackCell> >& field);
};

class ShotAI
{
public:
    ShotAI();
    QPair<int, int> chooseShot(Ship* ship, ShotResult result);

    QVector<QVector<AttackCell> > getField() const;
    void setField(const QVector<QVector<AttackCell> > &value);

private:
    //Field for saving information about shots and killed ships
    QVector<QVector<AttackCell>> field;
    // array of maximum number of shots to hit the target
    QVector<QPair<int, int>> possibleShots;
    // ways which is possible to shot after hit
    QVector<Direction> waysToShot;
    std::vector<int>* aliveShipSizes;

    int currentLongestShip;
    ComboShots* comboShots;
    QPair<int, int> previousShot;

    void setPossibleShots(int shipSize); // write all needed shots (x;y) for targeting an n-cell ship into 2d vector "possibleShots"
    void updatePossibleShots(Ship* ship); // update "poossibleShots" vector
    void excludeTriggeredCells(Ship* ship); // excluding triggered cells after shot
    void updateField(Ship *ship); // update field by excluding triggered cells
    void updateFieldCell(); // update last shoted cell
    void fillField(); // fills field with UNTRIGGERED cells
    QPair<int, int> chooseNextShot(ShotResult result);  // chooses next shot in refer to the result of previous shot
    QPair<int, int> choosePossibleShot();
    QPair<int, int> chooseComboShot();
    QPair<int, int> chooseComboShotWithoutDefinedDirection();
    QPair<int, int> chooseComboShotWithDefinedDirection(Direction dir);
    QPair<int, int> findNeighbourCellFromDirection(QPair<int, int> cell, Direction dir);
    Direction switchDirections(Direction dir);
};

#endif // SHOTAI_H
