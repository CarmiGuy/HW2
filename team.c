#include "team.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>

struct team
{
    char* name;
    Driver driver1;
    Driver driver2;
};

Team TeamCreate(TeamStatus* status, char* name)
{
    char* nameCopy = malloc(sizeof(char) * (strlen(name) + 1));
    Team newTeam = malloc(sizeof(Team*));
    if(newTeam == NULL || nameCopy == NULL)
    {
        *status = TEAM_MEMORY_ERROR;
        return NULL;
    }
    strcpy(nameCopy, name);
    newTeam->name = nameCopy;
    newTeam->driver1 = NULL;
    newTeam->driver2 = NULL;
    *status = TEAM_STATUS_OK;
    return newTeam;
}

void TeamDestroy(Team team)
{
    free(team->name);
    free(team->driver1);
    free(team->driver2);
    free(team);
}

TeamStatus TeamAddDriver(Team team, Driver driver)
{
    printf("TAD1\n");
    printf("%s\n", TeamGetName(team));
    if(team == NULL || driver == NULL)
    {
        return TEAM_NULL_PTR;
    }
    printf("TAD2\n");
    if(TeamGetDriver(team, FIRST_DRIVER) == NULL
       && TeamGetDriver(team, SECOND_DRIVER) == NULL)
    {
        printf("TAD3\n");
        team->driver1 = driver;
    }
    else if(TeamGetDriver(team, FIRST_DRIVER) != NULL
            && TeamGetDriver(team, SECOND_DRIVER) == NULL)
    {
        printf("TAD4\n");
        team->driver2 = driver;
    }
    else
    {
        printf("TAD5\n");
        return TEAM_FULL;
    }
    printf("TAD6\n");
    DriverSetTeam(driver, team); //MIGHT NEED TO CHANGE
    printf("TAD7\n");
    return TEAM_STATUS_OK;
}

const char * TeamGetName(Team team)
{
    if(team == NULL)
    {
        return NULL;
    }
    return team->name;
}

Driver TeamGetDriver(Team team, DriverNumber driver_number)
{
    if(team == NULL || (driver_number != FIRST_DRIVER && driver_number != SECOND_DRIVER))
    {
        return NULL;
    }
    else if(driver_number == FIRST_DRIVER)
    {
        return team->driver1;
    }
    return team->driver2;
}

int TeamGetPoints(Team team, TeamStatus *status)
{
    if(team == NULL)
    {
        *status = TEAM_NULL_PTR;
        return 0;
    }
    *status = TEAM_STATUS_OK;
    DriverStatus driverStatus1 = DRIVER_STATUS_OK, driverStatus2 = DRIVER_STATUS_OK;
    int firstDriverPoints = DriverGetPoints(team->driver1, &driverStatus1);
    int secondDriverPoints = DriverGetPoints(team->driver2, &driverStatus2);
    if(driverStatus1 == DRIVER_STATUS_OK && driverStatus2 == DRIVER_STATUS_OK)
    {
        return firstDriverPoints + secondDriverPoints;
    }
    else if(driverStatus1 == DRIVER_STATUS_OK && driverStatus2 == INVALID_DRIVER)
    {
        return firstDriverPoints;
    }
    return 0;
}
