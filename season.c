#include "season.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#define TEAM_NAME i
#define DRIVER_ONE i + 1
#define DRIVER_TWO i + 2

struct season
{
	int year;
	int numOfTeams;
	Team* teams;
	int numOfDrivers;
	Driver* drivers;
};

static void SeasonGenerate(Season season, SeasonStatus* status,const char* season_info);

Season SeasonCreate(SeasonStatus* status,const char* season_info)
{
	Season newSeason = malloc(sizeof(newSeason*));
	if(newSeason == NULL)
	{
		*status = SEASON_MEMORY_ERROR;
		return NULL;
	}
	if(season_info == NULL)
	{
		*status = BAD_SEASON_INFO;
		return NULL;
	}
	SeasonGenerate(newSeason, status, season_info);
	return newSeason;
}

static void SeasonGenerate(Season season, SeasonStatus* status,const char* season_info)
{
	int driverCount = 1;
	season->year = atoi(season_info[0]);
	for(int i = 1; season_info[i]; i += 3)
	{
		TeamStatus teamStatus = TEAM_STATUS_OK;
		Team newTeam = TeamCreate(&teamStatus, season_info[TEAM_NAME]);
		if(teamStatus == TEAM_MEMORY_ERROR)
		{
			*status = SEASON_MEMORY_ERROR;
			return;
		}
		DriverStatus driverStatus = DRIVER_STATUS_OK;
		Driver newDriver1 = DriverCreate(&driverStatus, season_info[DRIVER_ONE], driverCount++);
		if(driverStatus == DRIVER_MEMORY_ERROR)
		{
			*status = SEASON_MEMORY_ERROR;
			return;
		}
		Driver newDriver2 = DriverCreate(&driverStatus, season_info[DRIVER_TWO], driverCount++);
		if(driverStatus == DRIVER_MEMORY_ERROR)
		{
			*status = SEASON_MEMORY_ERROR;
			return;
		}
	}
}

void SeasonDestroy(Season season)
{
	for(int i = 0; i < season->numOfDrivers; i++)
	{
		DriverDestroy(season->drivers[i]);
	}
	free(season->drivers);
	for(int i = 0; i < season->numOfTeams; i++)
	{
		TeamDestroy(season->teams[i]);
	}
	free(season->teams);
	free(season);
}

Driver SeasonGetDriverByPosition(Season season, int position, SeasonStatus* status)
{
	if(season == NULL || position < 0 || position > season->numOfDrivers)
	{
		*status = INVALID_POSITION;
		return NULL;
	}
	Driver* rankedDrivers = SeasonGetDriversStandings(season);
	return rankedDrivers[position];
}

Driver* SeasonGetDriversStandings(Season season)
{

}

Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status)
{
	if(season == NULL || position < 0 || position > season->numOfTeams)
	{
		*status = INVALID_POSITION;
		return NULL;
	}
	Team* rankedTeams = SeasonGetTeamsStandings(season);
	return rankedTeams[position];
}

Team* SeasonGetTeamsStandings(Season season)
{

}

int SeasonGetNumberOfDrivers(Season season)
{
	return season->numOfDrivers;
}

int SeasonGetNumberOfTeams(Season season)
{
	return season->numOfTeams;
}

SeasonStatus SeasonAddRaceResult(Season season, int* results)
{
	if(season == NULL || results == NULL)
	{
		return SEASON_NULL_PTR;
	}
	for(int i = 0; i < season->numOfDrivers; i++)
	{
		DriverAddRaceResult(season->drivers[results[i]], i + 1);
	}
	return SEASON_OK;
}
