#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gameController = new GameController();

    ui->userTableView->horizontalHeader()->setDefaultSectionSize(CELL_SIZE);
    ui->userTableView->verticalHeader()->setDefaultSectionSize(CELL_SIZE);
    ui->userTableView->horizontalHeader()->hide();
    ui->userTableView->verticalHeader()->hide();
    ui->userTableView->setModel(gameController->getUserField());

    ui->aiTableView->horizontalHeader()->setDefaultSectionSize(CELL_SIZE);
    ui->aiTableView->verticalHeader()->setDefaultSectionSize(CELL_SIZE);
    ui->aiTableView->horizontalHeader()->hide();
    ui->aiTableView->verticalHeader()->hide();
    ui->aiTableView->setModel(gameController->getAiField());
    ui->playerLabel->hide();

    setConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayShips(FieldModel* field)
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (field->item(i,j)->data().value<Cell*>()->getType() == SHIP)
                field->item(i,j)->setBackground(QBrush(Qt::yellow, Qt::SolidPattern));
        }
    }
}

void MainWindow::hideShips(FieldModel* field)
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (field->item(i,j)->data().value<Cell*>()->getType() == SHIP)
                field->item(i,j)->setBackground(QBrush(Qt::darkCyan, Qt::SolidPattern));
        }
    }
}

void MainWindow::setConnections()
{
    connect(gameController, SIGNAL(cellTriggered(QPair<int,int>,Player)), this, SLOT(changeCellColor(QPair<int,int>,Player)));
    connect(gameController, SIGNAL(surroundShip(Ship*,Player)), this, SLOT(changeSurroundColor(Ship*,Player)));
    connect(gameController, SIGNAL(gameEnded(QString)), this, SLOT(gameFinished(QString)));
    connect(gameController, SIGNAL(playerLabelChanged(QString)), this, SLOT(changePlayerLabel(QString)));
}

void MainWindow::prepareNextGame()
{
    /*
     * Prepares the consecutive game
     */
    gameController = new GameController();
    ui->battleButton->show();
    ui->createFleet->show();
    ui->userTableView->setModel(gameController->getUserField());
    ui->aiTableView->setModel(gameController->getAiField());
}

void MainWindow::on_createFleet_clicked()
{
    ui->userTableView->setModel(gameController->createField(USER));
    displayShips(gameController->getUserField());
}

void MainWindow::on_aiTableView_clicked(const QModelIndex &index)
{
    gameController->gameLoop(QPair<int, int>(index.row(), index.column()));
}

void MainWindow::on_battleButton_clicked()
{
    /*
     * Starts the game
     */
    hideShips(gameController->getUserField());
    gameController->setGameStarted(true);
    ui->battleButton->hide();
    ui->createFleet->hide();
    ui->playerLabel->show();
}

void MainWindow::changeCellColor(QPair<int, int> cell, Player player)
{
    /*
     * Change the visual representation of ship' cells due to changes
     */
    FieldModel* field;
    if (player == USER) {
        field = gameController->getUserField();
    } else if (player == AI) {
        field = gameController->getAiField();
    }

    if (field->item(cell.first, cell.second)->data().value<Cell*>()->getType() == SHIP) {
        field->item(cell.first, cell.second)->setBackground(QBrush(Qt::yellow, Qt::SolidPattern));
    } else {
        field->item(cell.first, cell.second)->setBackground(QBrush(Qt::lightGray, Qt::SolidPattern));
    }
}

void MainWindow::changeSurroundColor(Ship *ship, Player player)
{
    /*
     * Change the visual representation of surround ship' cells due to changes
     */
    FieldModel* field;
    if (player == USER) {
        field = gameController->getUserField();
    } else if (player == AI) {
        field = gameController->getAiField();
    }

    for (int i = 0; i < ship->surroundCellCordinates.size(); i++) {
        field->item(ship->surroundCellCordinates[i].first,
                    ship->surroundCellCordinates[i].second)->setBackground(QBrush(Qt::lightGray, Qt::SolidPattern));
    }
}

void MainWindow::changePlayerLabel(QString label)
{
    ui->playerLabel->setText(label);
}

GameController *MainWindow::getGameController() const
{
    return gameController;
}

void MainWindow::setGameController(GameController *value)
{
    gameController = value;
}

void MainWindow::gameFinished(QString winner)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, QString("BattleShipGame"),
                                                                QString(winner + " won. Play again?"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        prepareNextGame();
        setConnections();
    } else {
        exit(0);
    }
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, QString("BattleShipGame"),
                                                                QString("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
        exit(0);
    }
}
