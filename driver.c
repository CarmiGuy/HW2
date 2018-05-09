#include "driver.h"
#include "team.h"
#include "season.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>

struct driver
{
    int driverId;
    char* driver_name;
    Team team;
    int points;
    Season season;
};

Driver DriverCreate(DriverStatus* status, char* driver_name, int driverId)
{
    char* nameCopy = malloc(sizeof(char) * (strlen(driver_name) + 1));
    Driver newDriver = malloc(sizeof(*newDriver));
    if (newDriver == NULL || nameCopy == NULL)
    {
        *status = DRIVER_MEMORY_ERROR;
        return NULL;
    }
    strncpy(nameCopy, driver_name, strlen(driver_name) + 1);
    newDriver->driver_name = nameCopy;
    newDriver->driverId = driverId;
    newDriver->team = NULL;
    newDriver->season = NULL;
    *status = DRIVER_STATUS_OK;
    return newDriver;
}

void DriverDestroy(Driver driver)
{
    free(driver->driver_name);
    free(driver);
}

const char* DriverGetName(Driver driver)
{
    if (driver == NULL)
    {
        return NULL;
    }
    return driver->driver_name;
}

int DriverGetId(Driver driver)
{
    if (driver == NULL)
    {
        return 0;
    }
    return driver->driverId;
}

Team DriverGetTeam(Driver driver)
{
    if (driver == NULL)
    {
        return NULL;
    }
    return driver->team;
}

void DriverSetTeam(Driver driver, Team team)
{
    driver->team = team;
}

void DriverSetSeason(Driver driver, Season season)
{
    driver->season = season;
    driver->points = 0;
}

DriverStatus DriverAddRaceResult(Driver driver, int position)
{
    if (position <= 0)
    {
        return INVALID_POSITION;
    }
    if (driver == NULL)
    {
        return INVALID_DRIVER;
    }
    if (driver->season == NULL)
    {
        return SEASON_NOT_ASSIGNED;
    }
    driver->points += (SeasonGetNumberOfDrivers(driver->season) - position);
    return DRIVER_STATUS_OK;
}

int DriverGetPoints(Driver driver, DriverStatus* status)
{
    if (driver == NULL)
    {
        *status = INVALID_DRIVER;
        return 0;
    }
    *status = DRIVER_STATUS_OK;
    return driver->points;
}