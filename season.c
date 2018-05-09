#include "season.h"
#include "team.h"
#include "driver.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>

#define TEAM_NAME i
#define DRIVER_ONE i + 1
#define DRIVER_TWO i + 2

struct season
{
	int year;
	int numOfTeams;
	Team *teams;
	int numOfDrivers;
	Driver *drivers;
};

static bool None(char* str);
static void SeasonGenerate(Season season, SeasonStatus* status, char* season_info);

static bool None(char* str)
{
	if (str[0] == 'N' && str[1] == 'o' && str[2] == 'n' && str[3] == 'e')
	{
		return true;
	}
	return false;

}

Season SeasonCreate(SeasonStatus* status, const char* season_info)
{
	if (status == NULL)
	{
		status = malloc(sizeof(status));
	}
	if (status == NULL)
	{
		return NULL;
	}
	*status = SEASON_OK;
	Season newSeason = malloc(sizeof(*newSeason));
	if (newSeason == NULL)
	{
		*status = SEASON_MEMORY_ERROR;
		return NULL;
	}
	if (season_info == NULL)
	{
		*status = BAD_SEASON_INFO;
		return NULL;
	}
	newSeason->numOfTeams = 0;
	newSeason->numOfDrivers = 0;
	newSeason->teams = NULL;
	newSeason->drivers = NULL;
	SeasonGenerate(newSeason, status, (char*)season_info);
	return newSeason;
}

static SeasonStatus handler(Season season, int *teamCounter, int *driverCounter, char *lineBuffer, int *purpose,
	SeasonStatus *status)
{
	if (*purpose)
	{
		if (1 == ((*purpose) % 3))
		{
			*purpose = (*purpose + 1);
			TeamStatus teamStatus = TEAM_STATUS_OK;
			Team newTeam = TeamCreate(&teamStatus, lineBuffer);
			if (SeasonGetNumberOfTeams(season) == 0)
			{
				season->teams = malloc(sizeof(Team));
				if (season->teams == NULL)
				{
					*status = SEASON_MEMORY_ERROR;
					return *status;
				}
			}
			else
			{
				season->teams = realloc(season->teams, ((*teamCounter + 1)) * sizeof(Team));
				if (teamStatus == TEAM_MEMORY_ERROR || season->teams == NULL)
				{
					*status = SEASON_MEMORY_ERROR;
					return *status;
				}
			}
			season->teams[*teamCounter] = newTeam;
			*teamCounter = (*teamCounter + 1);
			season->numOfTeams = (season->numOfTeams + 1);
		}
		else
		{
			*purpose = (*purpose + 1);
			if (((strlen(lineBuffer)) == 4) && (None(lineBuffer)))
			{
			}
			else
			{
				DriverStatus driverStatus = DRIVER_STATUS_OK;
				Driver newDriver = DriverCreate(&driverStatus, lineBuffer, (*driverCounter + 1));
				if (SeasonGetNumberOfDrivers(season) == 0)
				{
					season->drivers = malloc(sizeof(Driver));
					if (season->drivers == NULL)
					{
						*status = SEASON_MEMORY_ERROR;
						return *status;
					}
				}
				else
				{
					season->drivers = realloc(season->drivers, sizeof(Driver)*(*driverCounter + 1));
					if (driverStatus == DRIVER_MEMORY_ERROR || season->drivers == NULL)
					{
						*status = SEASON_MEMORY_ERROR;
						return *status;
					}
				}
				DriverSetSeason(newDriver, season);
				season->drivers[*driverCounter] = newDriver;
				TeamAddDriver(season->teams[*teamCounter - 1], season->drivers[*driverCounter]);
				*driverCounter = (*driverCounter + 1);
				season->numOfDrivers = (season->numOfDrivers + 1);
			}
		}
	}
	else
	{
		*purpose = (*purpose + 1);
		int begin = 0;
		while (lineBuffer[begin] == ' ')
		{
			begin++;
		}
		season->year = atoi(lineBuffer + begin);
	}
	*status = SEASON_OK;
	return *status;
}

void SeasonDestroy(Season season)
{
	for (int i = (season->numOfDrivers - 1); i >= 0; i--)
	{
		if (season->drivers[i] != NULL)
		{
		}
		DriverDestroy(season->drivers[i]);
	}
	if (season->drivers != NULL)
	{
		free(season->drivers);
	}
	for (int i = (season->numOfTeams - 1); i >= 0; i--)
	{
		TeamDestroy(season->teams[i]);
	}
	if (season->teams != NULL)
	{
		free(season->teams);
	}
	if (season != NULL)
	{
		free(season);
	}
}

static void FillString(char *dest, char *source, int begin, int count)
{
	for (int i = begin; i< count; i++)
	{
		dest[i - begin] = source[i];
	}
	dest[count - begin] = '\0';
}

static void SeasonGenerate(Season season, SeasonStatus* status, char* season_info)
{
	int count = 0, tempCount = 0;
	int purpose = 0, teamCounter = 0, driverCounter = 0;
	bool error = false;
	while ((count < (strlen(season_info) - 1) && (!error)))
	{
		tempCount = count;
		while (((season_info[count] != '\n') && (season_info[count] != EOF)) && (!error))
		{
			count++;
		}
		if ((count - tempCount) >= 0)
		{
			int size = (count - tempCount);
			if (size < 0)
			{
				size = 0;
			}
			char* lineBuffer = (char*)malloc((size + 1) * sizeof(char));
			if (lineBuffer == NULL)
			{
				*status = SEASON_MEMORY_ERROR;
				error = true;
			}
			else
			{
				FillString(lineBuffer, season_info, tempCount, count);
				handler(season, &teamCounter, &driverCounter, lineBuffer, &purpose, status);
				free(lineBuffer);
			}
		}
		count++;
	}
}


Driver SeasonGetDriverByPosition(Season season, int position, SeasonStatus* status)
{
	if (status == NULL)
	{
		status = malloc(sizeof(status));
	}
	if (status == NULL)
	{
		return NULL;
	}
	if (season == NULL || position <= 0 || position > season->numOfDrivers)
	{
		*status = SEASON_MEMORY_ERROR;
		return NULL;
	}
	Driver* rankedDrivers = SeasonGetDriversStandings(season);
	*status = SEASON_OK;
	return rankedDrivers[position - 1];
}

Driver* SeasonGetDriversStandings(Season season)
{
	if (season == NULL)
	{
		return NULL;
	}
	return season->drivers;
}

Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status)
{
	if (status == NULL)
	{
		status = malloc(sizeof(status));
	}
	if (status == NULL)
	{
		return NULL;
	}
	if (season == NULL || position <= 0 || position > season->numOfTeams)
	{
		*status = SEASON_MEMORY_ERROR;
		return NULL;
	}
	Team* rankedTeams = SeasonGetTeamsStandings(season);
	return rankedTeams[position - 1];
}

Team* SeasonGetTeamsStandings(Season season)
{
	if (season == NULL)
	{
		return NULL;
	}
	return  season->teams;
}

int SeasonGetNumberOfDrivers(Season season)
{
	if (season == NULL)
	{
		return 0;
	}
	return season->numOfDrivers;
}

int SeasonGetNumberOfTeams(Season season)
{
	if (season == NULL)
	{
		return 0;
	}
	return season->numOfTeams;
}

static void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

static void DriverCopy(Driver driver1, Driver driver2)
{
	Driver* tempDriver1 = &driver1;
	Driver * tempDriver2 = &driver2;
	Driver * tempDriver3 = tempDriver1;
	tempDriver1 = (Driver *)tempDriver2;
	tempDriver2 = (Driver *)tempDriver3;
}

static void TeamCopy(Team team1, Team team2)
{
	Team* tempTeam1 = &team1;
	Team * tempTeam2 = &team2;
	Team * tempTeam3 = tempTeam1;
	tempTeam1 = (Team *)tempTeam2;
	tempTeam2 = (Team *)tempTeam3;
}

static void BubbleSort(int objectNumber, int* points, int* desicionMaker, int* resultArr, Season season, char type)
{
	for (int i = 0; i < (objectNumber - 1); i++)
	{
		for (int j = 0; j < (objectNumber - i - 1); j++)
		{
			if ((points[j] < points[j + 1]) || (((points[j] == points[j + 1]) && desicionMaker[j] > desicionMaker[j + 1])))
			{
				if (type != 'D')
				{
					TeamCopy(season->teams[j], season->teams[j + 1]);
				}
				else
				{
					swap(&desicionMaker[j], &desicionMaker[j + 1]);
					/*DriverCopy(season->drivers[j], season->drivers[j + 1]);*/
				}
				swap(&points[j], &points[j + 1]);
				swap(&resultArr[j], &resultArr[j + 1]);
			}
		}

	}
}


SeasonStatus SeasonAddRaceResult(Season season, int* results)
{
	if (season == NULL || results == NULL || season->drivers == NULL || season->teams == NULL)
	{
		return SEASON_NULL_PTR;
	}
	int teamsNumber = SeasonGetNumberOfTeams(season);
	int driversNumber = SeasonGetNumberOfDrivers(season);
	int* oldPlacement = malloc(driversNumber * sizeof(int)); //filled by id, arranged by last race
	int* newPlacement = malloc(driversNumber * sizeof(int));
	int* newPosition = malloc(driversNumber * sizeof(int)); //filled w/ score on last race, arranged by ID
	int* points = malloc(driversNumber * sizeof(int));
	int* resReverse = malloc(driversNumber * sizeof(int));
	if (oldPlacement == NULL || newPlacement == NULL || newPosition == NULL || points == NULL || resReverse == NULL)
	{
		return SEASON_MEMORY_ERROR;
	}
	DriverStatus driverStatus = DRIVER_STATUS_OK;
	for (int i = 0; i < driversNumber; i++)
	{
		oldPlacement[i] = (DriverGetId(season->drivers[i]) - 1);
		newPlacement[i] = oldPlacement[i];
		resReverse[results[i] - 1] = (i + 1);
	}
	for (int i = 0; i < driversNumber; i++)
	{
		newPosition[oldPlacement[i]] = i;
	}
	for (int i = 0; i < driversNumber; i++)
	{
		DriverAddRaceResult(season->drivers[newPosition[results[i] - 1]], (i + 1));
		points[i] = DriverGetPoints(season->drivers[i], &driverStatus);
	}
	BubbleSort(driversNumber, points, resReverse, newPlacement, season, 'D');
	for (int i = 0; i < driversNumber; i++)
	{
		printf("op = %d, np= %d, driver= %s, pts  %d=\n", oldPlacement[i], newPlacement[i],
			DriverGetName(season->drivers[i]), points[i]);
	}
	for (int i = 0; i < driversNumber; i++)
	{
		newPosition[newPlacement[i]] = i;
	}
	for (int i = 0; i < driversNumber; i++)
	{
		if (DriverGetId(season->drivers[newPosition[i]]) != DriverGetId(season->drivers[i]))
		{
			DriverCopy((season->drivers[i]), (season->drivers[newPosition[i]]));
		}
	}
	int* teamThing = malloc(teamsNumber * sizeof(int));
	if (driversNumber < teamsNumber)
	{
		points = realloc(points, (teamsNumber - driversNumber)*(sizeof(int)));
	}
	if (teamThing == NULL || points == NULL)
	{
		free(newPlacement);
		free(newPosition);
		free(oldPlacement);
		free(points);
		free(teamThing);
		free(resReverse);
		return SEASON_MEMORY_ERROR;
	}
	for (int i = 0; i < teamsNumber; i++)
	{
		TeamStatus status = TEAM_STATUS_OK;
		points[i] = TeamGetPoints(season->teams[i], &status);
		if (TeamGetDriver(season->teams[i], FIRST_DRIVER) == NULL)
		{
			teamThing[i] = -1;
			points[i] = -1; /* if team has one driver w/ 0 points then it should be ranked higher
							* then team w/o drivers*/
		}
		else
		{
			teamThing[i] = newPosition[DriverGetId(TeamGetDriver(season->teams[i], FIRST_DRIVER))];
			if (((TeamGetDriver(season->teams[i], SECOND_DRIVER)) != NULL) &&
				(newPosition[DriverGetId(TeamGetDriver(season->teams[i], SECOND_DRIVER))] > teamThing[i]))
			{
				teamThing[i] = newPosition[DriverGetId(TeamGetDriver(season->teams[i], SECOND_DRIVER))];
			}
		}
	}
	BubbleSort(teamsNumber, points, teamThing, teamThing, season, 'T');
	free(newPlacement);
	free(newPosition);
	free(oldPlacement);
	free(teamThing);
	free(points);
	free(resReverse);
	return SEASON_OK;
}