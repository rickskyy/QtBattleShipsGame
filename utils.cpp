#include "utils.h"

Utils::Utils()
{

}

void Utils::delay(int seconds)
{
    /*
     * Freeze an application for a given number of seconds.
     */
    QTime dieTime= QTime::currentTime().addSecs(seconds);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

Player Utils::switchPlayer(Player player)
{
    if (player == USER) {
        return AI;
    } else {
        return USER;
    }
}

QString Utils::getPlayerName(Player player)
{
    if (player == USER) {
        return QString("You");
    } else {
        return QString("AI");
    }
}
