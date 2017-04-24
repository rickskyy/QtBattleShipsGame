#include "gamecontroller.h"

GameController::GameController(QObject *parent): QObject(parent)
{
    userField = new FieldModel(ROWS, COLS);
    userFleetController = new FleetController(userField);
    userField = userFleetController->createFleet(false);

    aiField = new FieldModel(ROWS, COLS);
    aiFleetController = new FleetController(aiField);
    aiField = aiFleetController->createFleet(true);

    ai = new ShotAI();
    lastAIResult.first = nullptr;
    lastAIResult.second = NO;

    gameFinished = false;
    gameStarted = false;
    aiFirstShot = true;
    userTurn = true;
    aiFinished = true;
}


FieldModel *GameController::createField(Player player)
{
    if (player == USER) {
        userField = new FieldModel(ROWS, COLS);
        userFleetController = new FleetController(userField);
        userField = userFleetController->createFleet(false);
        return userField;
    } else if (player == AI) {
        aiField = new FieldModel(ROWS, COLS);
        aiFleetController = new FleetController(aiField);
        aiField = aiFleetController->createFleet(true);
        return aiField;
    }
}

bool GameController::processAIShots()
{
    bool missed = false;
    while (!missed) {
        QPair<int, int> shot = ai->chooseShot(lastAIResult.first, lastAIResult.second);
        lastAIResult = makeShot(shot, AI, userField, userFleetController);
        if (lastAIResult.second == MISS)
            missed = true;
        Utils::delay(1);
    }

    return true;
}

bool GameController::processUserShots(QPair<int, int> cell)
{
    QPair<Ship*, ShotResult> res = makeShot(cell, USER, aiField, aiFleetController);
    if (res.second == MISS)
        userTurn = false;
    else
        userTurn = true;

    return userTurn;
}


QPair<Ship*, ShotResult> GameController::makeShot(QPair<int, int> cell, Player player,
                                                  FieldModel* field, FleetController* controller)
{
    /*
     * Makes a shot on a given field.
     * Emit signals to the window to mark visual changes on the field.
     * Returns data about the ship and shot result.
     */
    field->item(cell.first, cell.second)->data().value<Cell*>()->setTriggered(true);

    CellType cellType = field->item(cell.first, cell.second)->data().value<Cell*>()->getType();
    emit cellTriggered(cell, Utils::switchPlayer(player));

    if (cellType == SHIP) {
        QPair<Ship*, ShotResult> res = controller->findShip(cell);
        if (res.second == KILL) {
            emit surroundShip(res.first, Utils::switchPlayer(player));
        }
        if (controller->getFleet()->getFleetSunk()) {
            gameFinished = true;
            winner = player;
        }
        return res;
    } else if (cellType == EMPTY || cellType == SURROUND) {
        return QPair<Ship*, ShotResult>(nullptr, MISS);
    }
}

void GameController::gameLoop(QPair<int, int> cell)
{
    if (aiFinished && gameStarted) {
        if (userTurn) {
            emit playerLabelChanged(QString("User"));
            userTurn = processUserShots(cell);
        }

        if (!userTurn) {
            emit playerLabelChanged(QString("AI"));
            userTurn = true;
            aiFinished = false;
            aiFinished = processAIShots();
            emit playerLabelChanged(QString("User"));
        }
    }
    if (gameFinished)
        emit gameEnded(Utils::getPlayerName(winner));
}

FieldModel *GameController::getUserField() const
{
    return userField;
}

void GameController::setUserField(FieldModel *value)
{
    userField = value;
}

FieldModel *GameController::getAiField() const
{
    return aiField;
}

void GameController::setAiField(FieldModel *value)
{
    aiField = value;
}

FleetController *GameController::getUserFleetController() const
{
    return userFleetController;
}

void GameController::setUserFleetController(FleetController *value)
{
    userFleetController = value;
}

FleetController *GameController::getAiFleetController() const
{
    return aiFleetController;
}

void GameController::setAiFleetController(FleetController *value)
{
    aiFleetController = value;
}

bool GameController::getUserTurn() const
{
    return userTurn;
}

void GameController::setUserTurn(bool value)
{
    userTurn = value;
}

bool GameController::getGameStarted() const
{
    return gameStarted;
}

void GameController::setGameStarted(bool value)
{
    gameStarted = value;
}
