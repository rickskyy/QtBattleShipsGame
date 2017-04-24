#include "fleet.h"

Fleet::Fleet()
{
    fleetSunk = false;
}

bool Fleet::getFleetSunk() const
{
    return fleetSunk;
}

void Fleet::setFleetSunk(bool value)
{
    fleetSunk = value;
}

