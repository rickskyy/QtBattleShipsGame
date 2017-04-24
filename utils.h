#ifndef UTILS_H
#define UTILS_H

#include <QTime>
#include <QCoreApplication>
#include <QString>

#include <constants.h>

class Utils
{
public:
    Utils();

    static void delay(int seconds);
    static Player switchPlayer(Player player);
    static QString getPlayerName(Player player);
};

#endif // UTILS_H
