#include "shotai.h"

ShotAI::ShotAI()
{
    fillField();
    aliveShipSizes = new std::vector<int>(SHIP_SIZES, SHIP_SIZES + sizeof(SHIP_SIZES) / sizeof(SHIP_SIZES[0]));
    currentLongestShip = 4;
    setPossibleShots(currentLongestShip);
    comboShots = nullptr;
    previousShot.first = -1;
    previousShot.second = -1;
}

QPair<int, int> ShotAI::chooseShot(Ship *ship, ShotResult result)
{
    /*
     * Choose an optimal shot to shoot the target.
     * Updates field and vector of possible shots.
     */
    if (previousShot.first != -1)
        updateFieldCell();
    if (result == KILL) {
        updateField(ship);
        updatePossibleShots(ship);
    }
    return chooseNextShot(result);
}

QVector<QVector<AttackCell> > ShotAI::getField() const
{
    return field;
}

void ShotAI::setField(const QVector<QVector<AttackCell> > &value)
{
    field = value;
}

void ShotAI::setPossibleShots(int shipSize)
{
    /*
     * Set longest-ship-first tactic for AI. Tactic refers to killing the longest ship in the opponents' fleet first.
     * For finding and destroying the 4-cell ship AI will shoot randomly from the following positions:
     *   0 1 2 3 4 5 6 7 8 9
     * 0       *       *    
     * 1     *       *
     * 2   *       *
     * 3 *       *
     * 4 ...
     */
    possibleShots.clear();
    int step = shipSize;
    for (int row = 0; row < ROWS; row++) {
        for (int col = step-1; col < COLS; col += shipSize) {
            if(field[row][col] == UNTRIGGERED) {
                possibleShots.push_back(QPair<int, int>(row, col));
            }
        }
        step--;
        if (step == 0)
            step = shipSize;
    }
}

void ShotAI::updatePossibleShots(Ship *ship)
{
    /*
     * Updates possible shots vector due to the longest-ship-first tactic.
     */
    bool flag = true;
    int i = 0;

    std::vector<int>::iterator it = std::find(aliveShipSizes->begin(), aliveShipSizes->end(), ship->getDefaultShipSize());
    if (it != aliveShipSizes->end()) { // if found
        aliveShipSizes->erase(it);
    }

    if (currentLongestShip > ship->getDefaultShipSize()) {
        excludeTriggeredCells(ship);
    } else {
        while (flag && ((ship->getDefaultShipSize() - i) > 0)) {
            it = std::find(aliveShipSizes->begin(), aliveShipSizes->end(), ship->getDefaultShipSize() - i);
            if (it != aliveShipSizes->end() && i == 0) {
                excludeTriggeredCells(ship);
                flag = false;
            } else if (it != aliveShipSizes->end()) {
                setPossibleShots(ship->getDefaultShipSize() - i);
                currentLongestShip = ship->getDefaultShipSize() - i;
                flag = false;
            } else {
                i++;
            }
        }
    }
}

void ShotAI::excludeTriggeredCells(Ship *ship)
{
    for (int i = 0; i < ship->surroundCellCordinates.size(); i++) {
        possibleShots.removeOne(ship->surroundCellCordinates[i]);
    }
}

void ShotAI::updateField(Ship *ship)
{
    QPair<int, int> index;
    for (int i = 0; i < ship->surroundCellCordinates.size(); i++) {
        index = ship->surroundCellCordinates[i];
        field[index.first][index.second] = TRIGGERED;
    }

    for (int i = 0; i < ship->shipCoordinates.size(); i++) {
        index = ship->shipCoordinates[i];
        field[index.first][index.second] = TRIGGERED;
    }
}

void ShotAI::updateFieldCell()
{
    field[previousShot.first][previousShot.second] = TRIGGERED;
}

QPair<int, int> ShotAI::chooseNextShot(ShotResult result)
{
    /*
     * Chooses next shot for AI due to previous shot result.
     */
    srand(time(NULL));
    if (result == NO) {
        return choosePossibleShot();
    } else if (result == MISS ){
        if (!comboShots)
            return choosePossibleShot();
        else {
            if (comboShots->foundDirection == false)
                return chooseComboShotWithoutDefinedDirection();
            else { // if missed after shooting the ship
                return chooseComboShotWithDefinedDirection(comboShots->previousDirection);
            }
        }
    } else if (result == KILL) {
        delete(comboShots);
        comboShots = nullptr;
        return choosePossibleShot();
    } else if (result == SHOT) {
        if (!comboShots) {
            comboShots = new ComboShots(previousShot, field);
            return chooseComboShotWithoutDefinedDirection();
        } else { // if shoot multiple times in a row
            return chooseComboShot();
        }
    }
}

QPair<int, int> ShotAI::choosePossibleShot()
{
    srand(time(NULL));
    int index = rand() % possibleShots.size();
    previousShot = possibleShots[index];
    possibleShots.remove(index);
    return previousShot;
}

QPair<int, int> ShotAI::chooseComboShot()
{
    /*
     * Choose a shot when AI hitted multiple times in a row
     */
    comboShots->foundDirection = true;
    comboShots->findPossibleDirections(previousShot, field);
    if (comboShots->possibleDirections.contains(comboShots->previousDirection)) {
        previousShot = findNeighbourCellFromDirection(previousShot, comboShots->previousDirection);
        return previousShot;
    } else { // it is impossible to continue to shoot in the previous direction, because of end of field or triggered cell
        comboShots->previousDirection = switchDirections(comboShots->previousDirection);
        previousShot = findNeighbourCellFromDirection(comboShots->lastShotsPositions.front(), comboShots->previousDirection);
        comboShots->lastShotsPositions.push_back(previousShot);
        return previousShot;
    }
}

QPair<int, int> ShotAI::chooseComboShotWithoutDefinedDirection()
{
    /*
     * Choose a next shot after hitting the ship cell
     */
    int dirIndex = rand() % comboShots->possibleDirections.size();
    comboShots->previousDirection = comboShots->possibleDirections[dirIndex];
    comboShots->possibleDirections.remove(dirIndex);
    previousShot = findNeighbourCellFromDirection(comboShots->lastShotsPositions.front(), comboShots->previousDirection);
    comboShots->lastShotsPositions.push_back(previousShot);
    return previousShot;
}

QPair<int, int> ShotAI::chooseComboShotWithDefinedDirection(Direction dir)
{
    /*
     * Choose a next shot after finding the position of a ship.
     */
    comboShots->previousDirection = switchDirections(dir);
    comboShots->possibleDirections.removeOne(dir);
    previousShot = findNeighbourCellFromDirection(comboShots->lastShotsPositions.front(), comboShots->previousDirection);
    comboShots->lastShotsPositions.push_back(previousShot);
    return previousShot;
}

QPair<int, int> ShotAI::findNeighbourCellFromDirection(QPair<int, int> cell, Direction dir)
{
    switch (dir) {
    case LEFT:
        return QPair<int, int>(cell.first, cell.second - 1);
    case UP:
        return QPair<int, int>(cell.first - 1, cell.second);
    case RIGHT:
        return QPair<int, int>(cell.first, cell.second + 1);
    case DOWN:
        return QPair<int, int>(cell.first + 1, cell.second);
    default:
        break;
    }
}

Direction ShotAI::switchDirections(Direction dir)
{
    switch (dir) {
    case LEFT:
        return RIGHT;
    case RIGHT:
        return LEFT;
    case UP:
        return DOWN;
    case DOWN:
        return UP;
    default:
        break;
    }
}

void ShotAI::fillField()
{
    QVector<AttackCell> tmpRow;
    for (int row = 0; row < ROWS; row++) {
        tmpRow.clear();
        for (int col = 0; col < COLS; col++) {
            tmpRow.push_back(UNTRIGGERED);
        }
        field.push_back(tmpRow);
    }
}

ComboShots::ComboShots(QPair<int, int> firstHit, QVector<QVector<AttackCell> > &field)
{
    lastShotsPositions.push_back(firstHit);
    findPossibleDirections(firstHit, field);
    foundDirection = false;
    previousDirection = NONE;
}

void ComboShots::findPossibleDirections(QPair<int, int> cell, QVector<QVector<AttackCell> > &field)
{
    /*
     * Check if there are reasons and possibilities to shoot the neighbour cells
     */
    possibleDirections.clear();
    if (cell.second - 1 >= 0 && field[cell.first][cell.second - 1] == UNTRIGGERED)
        possibleDirections.push_back(LEFT);
    if (cell.first - 1 >= 0 && field[cell.first - 1][cell.second] == UNTRIGGERED)
        possibleDirections.push_back(UP);
    if (cell.second + 1 < COLS && field[cell.first][cell.second + 1] == UNTRIGGERED)
        possibleDirections.push_back(RIGHT);
    if (cell.first + 1 < ROWS && field[cell.first + 1][cell.second] == UNTRIGGERED)
        possibleDirections.push_back(DOWN);
}
