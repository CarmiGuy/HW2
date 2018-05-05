#include "season.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "team.h"
#include "driver.h"

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
static void SeasonGenerate(Season season, SeasonStatus* status,const char* season_info);
static void function(char* season_info1);

static bool None(char* str)
{
    if (str[0] == 'N' && str[1] == 'o' && str[2] == 'n' && str[3] == 'e')
    {
        return true;
    }
    else
    {
        for (int i=0; i< 4; i++)
        {
            printf("%d: %c\n", i, str[i]);
        }
    }
    return false;

}

Season SeasonCreate(SeasonStatus* status,const char* season_info)
{
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
    newSeason->teams = NULL;
    newSeason->drivers = NULL;
    SeasonGenerate(newSeason, status, season_info);
    return newSeason;
}

static void SeasonGenerate(Season season, SeasonStatus* status,const char* season_info)
{
    int count = 0;
    int purpose = 0, teamCounter = 0, driverCounter = 0;
    while (count < (strlen(season_info)-1))
    {
        int tempCount = count;
        char *lineBuffer = malloc(sizeof(char));
        while ((season_info[count] != '\n') && (season_info[count] != EOF)) {
            if (count)
            {
                lineBuffer = realloc(lineBuffer, sizeof(char));
            }
            if (lineBuffer == NULL) {
                printf("Error reallocating space for line buffer.");
                free(lineBuffer);
                exit(1);
            }

            lineBuffer[count-tempCount] = season_info[count];
            count++;

        }
        lineBuffer[count-tempCount] = '\0';
        if (purpose)
        {
            if (1 == (purpose%3))
            {
                purpose++;
                TeamStatus teamStatus = TEAM_STATUS_OK;

                Team newTeam = TeamCreate(&teamStatus, lineBuffer);
                season->teams = realloc(season->teams, sizeof(Team*));
                if(teamStatus == TEAM_MEMORY_ERROR || season->teams == NULL)
                {
                    *status = SEASON_MEMORY_ERROR;
                    free(lineBuffer);
                    break;
                }
                season->teams[teamCounter] = newTeam;
                printf("season team %d = %s\n",(teamCounter+1),lineBuffer);
                teamCounter++;
            }
            else
            {
                purpose++;
                if (((count-tempCount) == 4) && (None(lineBuffer)))
                {
                    printf("catch none\n");
                }
                else
                {
                    DriverStatus driverStatus = DRIVER_STATUS_OK;
                    Driver newDriver = DriverCreate(&driverStatus, lineBuffer, (driverCounter+1));
                    season->drivers = realloc(season->drivers, sizeof(Driver*));
                    if(driverStatus == DRIVER_MEMORY_ERROR || season->drivers == NULL)
                    {
                        *status = SEASON_MEMORY_ERROR;
                        free(lineBuffer);
                        break;
                    }
                    DriverSetSeason(newDriver, season);
                    season->drivers[driverCounter] = newDriver;
                    TeamAddDriver(season->teams[teamCounter-1], season->drivers[driverCounter]);
                    printf("season driver %d = %s\n",(driverCounter+1),lineBuffer);
                    driverCounter++;
                }
            }
        }
        else
        {
            purpose++;
            int begin = 0;
            while (lineBuffer[begin] == ' ')
            {
                begin++;
            }
            season->year = atoi(lineBuffer+begin);
            printf("season year = %d\n",atoi(lineBuffer+begin));

        }
        free(lineBuffer);
        count++;
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

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void merge(int arr[], int l, int m, int r, int* oldPlacement, int* arr2)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr2[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr2[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] < R[j])
        {
            arr[k] = oldPlacement[l+i];
            i++;
        }
        else if (L[i] == R[j])
        {
            if (oldPlacement[i+ l] > oldPlacement[m + 1+ j])
            {
                arr[k] = oldPlacement[l+i];
                i++;
            }
            else
            {
                arr[k] = oldPlacement[m + 1+ j];
                j++;
            }
        }
        else
        {
            arr[k] = oldPlacement[m + 1+ j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = oldPlacement[l+i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = oldPlacement[m + 1+ j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r, int* oldPlacement, int* arr2)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(arr, l, m, oldPlacement, arr2);
        mergeSort(arr, m+1, r, oldPlacement, arr2);

        merge(arr, l, m, r, oldPlacement, arr2);
    }
}

void DriverCopy(Driver driver1, Driver driver2)
{
    Driver tempDriver = driver1;
    driver1 = driver2;
    driver2 = tempDriver;
}

SeasonStatus SeasonAddRaceResult(Season season, int* results)
{
    if(season == NULL || results == NULL)
    {
        return SEASON_NULL_PTR;
    }
    int teamsNumber = SeasonGetNumberOfTeams(season);
    int driversNumber = SeasonGetNumberOfDrivers(season);
    int* oldPlacement = malloc(driversNumber* sizeof(int)); //filled by id, arranged by last race
    int* newPlacement = malloc(driversNumber* sizeof(int));
    int* newPosition = malloc(driversNumber* sizeof(int)); //filled w/ score on last race, arranged by ID
    int* points = malloc(driversNumber* sizeof(int));
    if (oldPlacement == NULL || newPlacement == NULL || newPosition == NULL|| points == NULL)
    {
        return SEASON_MEMORY_ERROR;
    }
    printf("SARR1!!!\n");
    DriverStatus driverStatus = DRIVER_STATUS_OK;
    for (int i = 0; i < driversNumber; i++)
    {
        oldPlacement[i] = (DriverGetId(season->drivers[i])-1);
        newPlacement[i] = oldPlacement[i];
    }
    printf("SARR2!!!\n");
    for(int i = 0; i < driversNumber; i++)
    {
        DriverAddRaceResult(season->drivers[results[i]], (i + 1));
        points[i] = DriverGetPoints(season->drivers[i], &driverStatus);
    }
    printf("SARR3!!!\n");
    mergeSort(newPlacement, 0, (driversNumber-1), oldPlacement, points);
    printf("SARR4!!!\n");
    for (int i = 0; i < SeasonGetNumberOfDrivers(season); i++)
    {
        printf("op = %d, np= %d, driver= %s\n", oldPlacement[i], newPlacement[i], DriverGetName(season->drivers[i]));
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
    if (teamThing == NULL)
    {
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
    for (int i = 0; i < (teamsNumber -1); i++)
    {
        for (int j = 0; j < (teamsNumber -i -1); j++)
        {
            if ((points[j] > points[j+1]) || (((points[j] == points[j+1]) && teamThing[j] > teamThing[j+1])))
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


