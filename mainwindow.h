#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QModelIndex>
#include <QDebug>
#include <QCloseEvent>

#include "gamecontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    GameController *getGameController() const;
    void setGameController(GameController *value);

private slots:
    void on_createFleet_clicked();
    void on_aiTableView_clicked(const QModelIndex &index);
    void on_battleButton_clicked();

public slots:
    void changeCellColor(QPair<int, int> cell, Player player);
    void changeSurroundColor(Ship* ship, Player player);
    void gameFinished(QString winner);
    void changePlayerLabel(QString label);

private:
    GameController* gameController;
    Ui::MainWindow *ui;

    void displayShips(FieldModel* );
    void hideShips(FieldModel* );
    void setConnections();
    void prepareNextGame();

    void closeEvent (QCloseEvent *event);
};

#endif // MAINWINDOW_H
