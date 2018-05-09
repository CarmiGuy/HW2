#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "season.h"
#include "team.h"
#include "driver.h"

void Test(){
    printf(" -* Starting Test 1 *-\n");
    /** Driver's name is including "None" (btw, those are real names) */
    SeasonStatus st;
    char* season_info1="\
7\n\
Ferrari\n\
Noned Roy\n\
Its Britney bitch\n\
wtf\n\
None\n\
OINone\n\
Team Name With None\n\
GGG\n\
None\n\
";
    printf("Size = ");
    char* a = malloc(2);
    free(a);
    int size = strlen(season_info1);
    printf("%d\n", size);
    /*  !_-TeaMname55 !_-TeaMname55*/
    Season season = SeasonCreate(&st,season_info1);
    if (st != SEASON_OK)
    {
        printf("status = errr\n");
    }
/*    int race1[4] = {2,3,4,1};
    SeasonAddRaceResult(season, race1);
    int race2[4] = {1,3,4,2};
    SeasonAddRaceResult(season, race2);
    printf("Winner driver .......(OINone): %s\n", DriverGetName(SeasonGetDriverByPosition(season,1,&st)));
    printf("2nd driver .......(Noned Roy): %s\n", DriverGetName(SeasonGetDriverByPosition(season,2,&st)));
    printf("2nd team ..(wtf): %s\n", TeamGetName(SeasonGetTeamByPosition(season,2,&st))); */
    SeasonDestroy(season);
/*    printf("\n -* Starting Test 2A *-\n");
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

    SeasonDestroy(season2);*/
    printf("\nFinished! Check your score above.\n");
}

int main(){
    Test();
    return 0;
}

