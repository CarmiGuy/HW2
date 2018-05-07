#include "season.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "team.h"
#include "driver.h"
#include "malloc.h"

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

Season SeasonCreate(SeasonStatus* status,const char* season_info)
{
    *status = SEASON_OK;
    Season newSeason = malloc(sizeof(Season*));
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
    newSeason->numOfTeams =0;
    newSeason->numOfDrivers =0;
    newSeason->teams = NULL;
    newSeason->drivers = NULL;
    printf("SC is fine\n");
    SeasonGenerate(newSeason, status, (char *) season_info);
    return newSeason;
}

static SeasonStatus handler(Season season, int *teamCounter, int *driverCounter, char *lineBuffer, int *purpose,
                            SeasonStatus *status)
{
    if (*purpose)
    {
        if (1 == ((*purpose)%3))
        {
            *purpose = (*purpose + 1);
            TeamStatus teamStatus = TEAM_STATUS_OK;
            Team newTeam = TeamCreate(&teamStatus, lineBuffer);
            season->teams = realloc(season->teams, sizeof(Team*));
            if(teamStatus == TEAM_MEMORY_ERROR || season->teams == NULL)
            {
                *status = SEASON_MEMORY_ERROR;
                return *status;
            }
            season->teams[*teamCounter] = newTeam;
            *teamCounter = (*teamCounter + 1);
            season->numOfTeams = (season->numOfTeams +1);
        }
        else
        {
            *purpose = (*purpose + 1);
            if (((strlen(lineBuffer)) == 4) && (None(lineBuffer)))
            {
                printf("catch none\n");
            }
            else
            {
                DriverStatus driverStatus = DRIVER_STATUS_OK;
                Driver newDriver = DriverCreate(&driverStatus, lineBuffer, (*driverCounter+1));
                season->drivers = realloc(season->drivers, sizeof(Driver*));
                if(driverStatus == DRIVER_MEMORY_ERROR || season->drivers == NULL)
                {
                    *status = SEASON_MEMORY_ERROR;
                    return *status;
                }
                DriverSetSeason(newDriver, season);
                season->drivers[*driverCounter] = newDriver;
                TeamAddDriver(season->teams[*teamCounter-1], season->drivers[*driverCounter]);
                *driverCounter= (*driverCounter+ 1);
                season->numOfDrivers = (season->numOfDrivers +1);
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
        season->year = atoi(lineBuffer+begin);
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

static void SeasonGenerate(Season season, SeasonStatus* status, char* season_info)
{
    int count = 0, stringSize = 1;
    int purpose = 0, teamCounter = 0, driverCounter = 0;
    bool error = false;
    while ((count < (strlen(season_info)-1) && (!error)))
    {
        int tempCount = count;
        char *lineBuffer = NULL;
        lineBuffer = calloc(1, sizeof(char));
        printf("2");
        if (lineBuffer == NULL)
        {
            *status = SEASON_MEMORY_ERROR;
            error = true;
            break;
        }
        stringSize=1;
        printf("DYE!\n");
        while (((season_info[count] != '\n') && (season_info[count] != EOF)) && (!error))
        {
            lineBuffer = realloc(lineBuffer,(1+stringSize)* sizeof(char));
            stringSize++;
            if (lineBuffer == NULL)
            {
                printf("Error reallocating space for line buffer.");
                *status = SEASON_MEMORY_ERROR;
                error = true;
                break;
            }
            lineBuffer[count-tempCount] = season_info[count];
            count++;
        }
        if (lineBuffer && (stringSize - (count-teamCounter) >=0 ))
        {
            lineBuffer[count - tempCount] = '\0';
            handler(season, &teamCounter, &driverCounter, lineBuffer, &purpose, status);
        }
        printf("%s\n", lineBuffer);
        free(lineBuffer);
        count++;
        printf("111111\n");
    }

    printf("succesfully created season!\n");
}


Driver SeasonGetDriverByPosition(Season season, int position, SeasonStatus* status)
{
    if(season == NULL || position <= 0 || position > season->numOfDrivers)
    {
        *status = SEASON_MEMORY_ERROR;
        return NULL;
    }
    Driver* rankedDrivers = SeasonGetDriversStandings(season);
    return rankedDrivers[position-1];
}

Driver* SeasonGetDriversStandings(Season season)
{
    return season->drivers;
}

Team SeasonGetTeamByPosition(Season season, int position, SeasonStatus* status)
{
    if(season == NULL || position <= 0 || position > season->numOfTeams)
    {
        *status = SEASON_MEMORY_ERROR;
        return NULL;
    }
    Team* rankedTeams = SeasonGetTeamsStandings(season);
    return rankedTeams[position-1];
}

Team* SeasonGetTeamsStandings(Season season)
{
    return  season->teams;
}

int SeasonGetNumberOfDrivers(Season season)
{
    return season->numOfDrivers;
}

int SeasonGetNumberOfTeams(Season season)
{
    return season->numOfTeams;
}

static void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
void DriverCopy(Driver driver1, Driver driver2)
{
    Driver tempDriver = driver1;
    driver1 = driver2;
    driver2 = tempDriver;
}

void TeamCopy(Team team1, Team team2)
{
    Team tempTeam = team1;
    team1 = team2;
    team2 = tempTeam;
}

static void BubbleSort(int objectNumber, int* points, int* desicionMaker, int* resultArr, Season season, char type)
{
    assert(season != NULL);
    for (int i = 0; i < (objectNumber -1); i++)
    {
        for (int j = 0; j < (objectNumber -i -1); j++)
        {
            if ((points[j] < points[j+1]) || (((points[j] == points[j+1]) && desicionMaker[j] > desicionMaker[j+1])))
            {
                if (type != 'D')
                {
                    TeamCopy(season->teams[j], season->teams[j+1]);
                }
                swap (&points[j], &points[j+1]);
                swap(&resultArr[j], &resultArr[j+1]);
            }
        }

    }
}


SeasonStatus SeasonAddRaceResult(Season season, int* results)
{
    if(season == NULL || results == NULL || season->drivers == NULL || season->teams == NULL)
    {
        return SEASON_NULL_PTR;
    }
    int teamsNumber = SeasonGetNumberOfTeams(season);
    int driversNumber = SeasonGetNumberOfDrivers(season);
    int* oldPlacement = malloc(driversNumber* sizeof(int)); //filled by id, arranged by last race
    int* newPlacement = malloc(driversNumber* sizeof(int));
    int* newPosition = malloc(driversNumber* sizeof(int)); //filled w/ score on last race, arranged by ID
    int* points = malloc(driversNumber* sizeof(int));
    int* resReverse = malloc(driversNumber* sizeof(int));
    if (oldPlacement == NULL || newPlacement == NULL || newPosition == NULL|| points == NULL || resReverse)
    {
        return SEASON_MEMORY_ERROR;
    }
    DriverStatus driverStatus = DRIVER_STATUS_OK;
    for (int i = 0; i < driversNumber; i++)
    {
        oldPlacement[i] = (DriverGetId(season->drivers[i])-1);
        newPlacement[i] = oldPlacement[i];
        resReverse[results[i]-1] = (i+1);
    }
    for(int i = 0; i < driversNumber; i++)
    {
        DriverAddRaceResult(season->drivers[results[i]], (i + 1));
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
        if (DriverGetId(season->drivers[newPosition[i]])  != DriverGetId(season->drivers[i]))
        {
            DriverCopy((season->drivers[i]), (season->drivers[newPosition[i]]));
        }
    }
    /* change team position. */
    int* teamThing = malloc(teamsNumber* sizeof(int));
    if (driversNumber < teamsNumber)
    {
        points = realloc(points, (teamsNumber-driversNumber)*(sizeof(int)));
    }
    if (teamThing == NULL || points == NULL)
    {
        free(newPlacement);
        free(newPosition);
        free(oldPlacement);
        free(points);
        free(teamThing);
        return SEASON_MEMORY_ERROR;
    }
    for(int i = 0; i < teamsNumber; i++)
    {
        TeamStatus status = TEAM_STATUS_OK;
        points[i] = TeamGetPoints(season->teams[i], &status);
        if (TeamGetDriver(season->teams[i], FIRST_DRIVER)== NULL)
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
    for (int i = 0; i < (teamsNumber -1); i++)
    {
        for (int j = 0; j < (teamsNumber -i -1); j++)
        {
            if ((points[j] < points[j+1]) || (((points[j] == points[j+1]) && teamThing[j] > teamThing[j+1])))
            {
                Team temp = season->teams[j];
                season->teams[j] = season->teams[j+1];
                season->teams[j+1] = temp;
                swap (&points[j], &points[j+1]);
                swap(&teamThing[j], &teamThing[j+1]);
            }
        }

    }
    free(newPlacement);
    free(newPosition);
    free(oldPlacement);
    free(teamThing);
    free(points);
    return SEASON_OK;
}


