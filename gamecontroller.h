#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QVector>
#include <QPair>
#include <QMessageBox>

#include "constants.h"
#include "fleetcontroller.h"
#include "shotai.h"
#include "utils.h"

class GameController: public QObject
{
    Q_OBJECT
public:
    explicit GameController(QObject* parent = 0);

    FieldModel* createField(Player player);
    void gameLoop(QPair<int, int> cell);
    bool processUserShots(QPair<int, int> cell);
    bool processAIShots();

    FieldModel *getUserField() const;
    void setUserField(FieldModel *value);
    FieldModel *getAiField() const;
    void setAiField(FieldModel *value);
    FleetController *getUserFleetController() const;
    void setUserFleetController(FleetController *value);
    FleetController *getAiFleetController() const;
    void setAiFleetController(FleetController *value);
    bool getUserTurn() const;
    void setUserTurn(bool value);
    bool getGameStarted() const;
    void setGameStarted(bool value);

signals:
    void cellTriggered(QPair<int, int> cell, Player player);
    void surroundShip(Ship* ship, Player player);
    void gameEnded(QString winner);
    void playerLabelChanged(QString label);

private:
    FieldModel* userField;
    FieldModel* aiField;
    FleetController* userFleetController;
    FleetController* aiFleetController;
    
    ShotAI* ai;
    bool gameFinished;
    bool gameStarted;
    bool aiFirstShot;
    bool userTurn;
    bool aiFinished;
    QPair<Ship*, ShotResult> lastAIResult;
    Player winner;

    QPair<Ship*, ShotResult> makeShot(QPair<int, int> cell, Player player, FieldModel *field, FleetController *controller);
    
};

#endif // GAMECONTROLLER_H
