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
	Driver Lewis = DriverCreate(&driver_status, "Lewis", 44);
	Team Mercedes = TeamCreate(&team_status, "Mercedes");
	DriverSetTeam(Lewis, Mercedes);
	assert(strcmp(DriverGetName(Lewis),"Lewis")==0);
	assert(strcmp(TeamGetName(DriverGetTeam(Lewis)),"Mercedes")==0);


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

	SeasonAddRaceResult(season, race_results);

	SeasonDestroy(season);
	printf("Finished Testing Driver!\n");
}
void TeamUnitTest(){
}
void SeasonUnitTest(){
}

int main(){
	printf("starting to run the unit tests!\n");
	DriverUnitTest();
	TeamUnitTest();
	SeasonUnitTest();
	return 0;
}
