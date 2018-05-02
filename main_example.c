/*
 * test_eclipce.c
 *
 *  Created on: Apr 9, 2018
 *      Author: muhamadg
 */

#include <stdio.h>
#include <stdlib.h>
#include "driver.h"
#include "team.h"
#include "season.h"
#include <assert.h>
#include <string.h>

void DriverUnitTest(){
    printf("Testing Driver!\n");
    DriverStatus driver_status;
    TeamStatus team_status;
 /*   Driver Lewis = DriverCreate(&driver_status, "Lewis", 44);
    Team Mercedes = TeamCreate(&team_status, "Mercedes");
    DriverSetTeam(Lewis, Mercedes);
    assert(strcmp(DriverGetName(Lewis),"Lewis")==0);
    assert(strcmp(TeamGetName(DriverGetTeam(Lewis)),"Mercedes")==0);
*/

    /*now lets try to create a season
    all drivers/teams created above are not related to the season.*/

    SeasonStatus season_status;
    char* season_info="\
2018\n\
Ferrari\n\
Sebastian Vettel\n\
Kimi Raikonen\n\
Mercedes\n\
Lewis Hamilton\n\
Valtteri Bottas\n\
RedBull Racing\n\
Daniel\n\
Max  Verstappen\n\
McLaren\n\
Fernando Alonso\n\
None\n\
";
    Season season = SeasonCreate(&season_status,season_info);
    int race_results[7] = {7, 1,3,2,4,5,6};

    SeasonStatus seasonStatus1 = SEASON_OK;
    SeasonAddRaceResult(season, race_results);
    for (int i = 0; i < SeasonGetNumberOfTeams(season); i++)
    {
        Team team = SeasonGetTeamByPosition(season, i, &seasonStatus1);
        if (seasonStatus1 != SEASON_OK)
        {
            printf("bug!!\n");
        }
        else
        {
            printf("team name = %s, position is = %d\n", TeamGetName(team), i);
        }
    }
    SeasonDestroy(season);
    printf("Finished Testing Driver!\n");
}

void Test(){
    printf(" -* Starting Test 1 *-\n");
    /** Driver's name is including "None" (btw, those are real names) */
    SeasonStatus st;
    char* season_info1="\
7\n\
Ferrari\n\
Noned Roy\n\
Its Britney bitch\n\
wtf!_-TeaMname55\n\
None\n\
OINone\n\
Team Name With None\n\
GGG\n\
None\n\
";
    Season season = SeasonCreate(&st,season_info1);
    int race1[4] = {2,3,4,1};
    SeasonAddRaceResult(season, race1);
    int race2[4] = {1,3,4,2};
    SeasonAddRaceResult(season, race2);
    printf("Winner driver .......(OINone): %s\n", DriverGetName(SeasonGetDriverByPosition(season,1,&st)));
    printf("2nd driver .......(Noned Roy): %s\n", DriverGetName(SeasonGetDriverByPosition(season,2,&st)));
    printf("2nd team ..(wtf!_-TeaMname55): %s\n", TeamGetName(SeasonGetTeamByPosition(season,2,&st)));
    SeasonDestroy(season);
    printf("\n -* Starting Test 2A *-\n");
    char* season_info2="\
7777557\n\
SUSITA FTW\n\
a\n\
BBB\n\
Noners\n\
CCC\n\
dog kisser\n\
RedBull Poison\n\
eee\n\
The Crusher\n\
Real winners drink wine\n\
g g g g g g\n\
None\n\
";
    Season season2 = SeasonCreate(&st,season_info2);
    int race_results[7] = {4, 1,3,2,7,5,6};
    SeasonAddRaceResult(season2, race_results);
    int race_results2[7] = {1, 3,6,4,5,7,2};
    SeasonAddRaceResult(season2, race_results2);
    printf("3rd driver ......(dog kisser): %s\n", DriverGetName(SeasonGetDriverByPosition(season2,3,&st)));
    printf("5th driver .............(eee): %s\n", DriverGetName(SeasonGetDriverByPosition(season2,5,&st)));
    printf("2nd team ........(SUSITA FTW): %s\n", TeamGetName(SeasonGetTeamByPosition(season2,2,&st)));
    printf(" -* Starting Test 2B *-\n");
    int race_results3[7] = {5,6,7,2,1,3,4};
    SeasonAddRaceResult(season2, race_results3);
    printf("Winner driver ............(a): %s\n", DriverGetName(SeasonGetDriverByPosition(season2,1,&st)));
    printf("6th driver .....(g g g g g g): %s\n", DriverGetName(SeasonGetDriverByPosition(season2,6,&st)));
    printf("Winner team .....(SUSITA FTW): %s\n", TeamGetName(SeasonGetTeamByPosition(season2,1,&st)));
    printf(" -* Starting Test 2C *-\n");
    int race_results4[7] = {6,7,4,1,2,3,5};
    SeasonAddRaceResult(season2, race_results4);
    printf("Winner driver ............(a): %s\n", DriverGetName(SeasonGetDriverByPosition(season2,1,&st)));
    printf("Winner team .(RedBull Poison): %s\n", TeamGetName(SeasonGetTeamByPosition(season2,1,&st)));
    printf("2nd team ............(Noners): %s\n", TeamGetName(SeasonGetTeamByPosition(season2,2,&st)));

    SeasonDestroy(season2);
    printf("\nFinished! Check your score above.\n");
}

/*void TeamUnitTest(){
}
void SeasonUnitTest(){
}
*/
int main(){
    printf("starting to run the unit tests!\n");
    Test();
    DriverUnitTest();
/*    TeamUnitTest();
    SeasonUnitTest(); */
    return 0;
}

