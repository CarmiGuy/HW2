#include "driver.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>

struct driver
{
	int driverId;
	char* driver_name;
	Team team; // should be a pointer??
	int points;
	Season season; //same?
};

Driver DriverCreate(DriverStatus* status, char* driver_name, int driverId)
{
	char* nameCopy = malloc(sizeof(driver_name*) * (strlen(driver_name) + 1));
	Driver newDriver = malloc(sizeof(newDriver));
	if(newDriver == NULL || nameCopy == NULL)
	{
		*status = DRIVER_MEMORY_ERROR;
		return NULL;
	}
	strcpy(nameCopy, driver_name);
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
	free(driver->team);
	free(driver->season);
}

const char* DriverGetName(Driver driver)
{
	if(driver == NULL)
	{
		return NULL;
	}
	return driver->driver_name;
}

int DriverGetId(Driver driver)
{
	if(driver == NULL)
	{
		return NULL;
	}
	return driver->driverId;
}

Team DriverGetTeam(Driver driver)
{
	if(driver == NULL)
	{
		return NULL;
	}
	return driver->team;
}

void DriverSetTeam(Driver driver, Team team)
{
	driver->team = team;//MIGHT NEED TO CHANGE (no kidding :) )
}

void DriverSetSeason(Driver driver, Season season)
{
	driver->season = season;
	driver->points = 0;
}

DriverStatus DriverAddRaceResult(Driver driver, int position)
{
	if(position < 0)
	{
		return INVALID_POSITION;
	}
	if(driver == NULL)
	{
		return INVALID_DRIVER;
	}
	if(driver->season == NULL)
	{
		return SEASON_NOT_ASSIGNED;
	}
	driver->points += SeasonGetNumberOfDrivers(driver->season) - position;
	return DRIVER_STATUS_OK;
}

int DriverGetPoints(Driver driver, DriverStatus* status)
{
	if(driver == NULL)
	{
		*status = INVALID_DRIVER;
		return 0;
	}
	*status = DRIVER_STATUS_OK;
	return driver->points;
}

void DriverCopy(driver* driver1, driver* driver2)
{
	driver* tempDriver = driver1;
	driver1 = driver2;
	driver2 = tempDriver;
}


