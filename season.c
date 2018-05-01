#include "season.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "team.h"
#include "driver.h"

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
	return season->drivers;
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
	return season->teams;
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
	/* season->drivers[i]->driverID == driver at place i   */
	int* oldPlacement = malloc((season->numOfDrivers)* sizeof(int)); //filled by id, arranged by last race
	int* newPlacement = malloc((season->numOfDrivers)* sizeof(int));
	int* newPosition = malloc((season->numOfDrivers)* sizeof(int)); //filled w/ score on last race, arranged by ID
	int* points = malloc((season->numOfDrivers)* sizeof(int));
	newPosition = { 0 };
	if (oldPlacement == NULL || newPlacement == NULL || newPosition == NULL|| points == NULL)
	{
		return SEASON_MEMORY_ERROR;
	}
	for (int i = 0; i < season->numOfDrivers; i++)
	{
		oldPlacement[i] = (season->drivers->driverID-1);
		newPlacement[i] = oldPlacement[i];
		/*temporary - same complexity */
	}

	for(int i = 0; i < season->numOfDrivers; i++)
	{
		DriverAddRaceResult(season->drivers[results[i]], i + 1);
		points[i] = season->drivers[i]->points;
	}
	mergeSort(newPlacement, 0, (season->numOfDrivers-1), oldPlacement, points);
	for (int i = 0; i < season->numOfDrivers; i++)
	{
		newPosition[newPlacement[i]] = i;
	}
	for (int i = 0; i < season->numOfDrivers; i++)
	{
		if (season->drivers[newPosition[i]]->driverID  != season->drivers[i]->driverID)
		{
			Driver* temp = season->drivers[i];
			season->drivers[i] = season->drivers[newPosition[i]];
			season->drivers[newPosition[i]] = temp;
		}
	}
	/* change team position. */
	int* teamThing = malloc((season->numOfTeams)* sizeof(int));
	if (teamThing == NULL)
	{
		return SEASON_MEMORY_ERROR;
	}
	for(int i = 0; i < season->numOfTeams; i++)
	{
		teamStatus *status = TEAM_STATUS_OK;
		points[i] = TeamGetPoints(season->teams[i], status);
		if (season->teams[i]->driver1 == NULL)
		{
			teamThing[i] = -1;
			points[i] = -1; /* if team has one driver w/ 0 points then it should be ranked higher
 							* then team w/o drivers*/
		}
		else
		{
			teamThing[i] = newPosition[season->teams[i]->driver1->driverID];
			if (season->teams[i]->driver2 != NULL && newPosition[season->teams[i]->driver2->driverID] > teamThing[i])
			{
				teamThing[i] = newPosition[season->teams[i]->driver2->driverID];
			}
		}
	}
	for (int i = 0; i < (season->numOfTeams -1); i++)
	{
		for (int j = 0; j < (season->numOfTeams -i -1); j++)
		{
			if ((points[j] > points[j+1]) || (((points[j] == points[j+1]) && teamThing[j] > teamThing[j+1])))
			{
				Team* temp = season->teams[j];
				season->teams[j] = season->teams[j+1];
				season->teams[j+1] = temp;
				swap (&points[j], &points[j+1]);
				swap(&teamThing[j], &teamThing[j+1]);
			}
		}

	}

/*	TODO: mergesort teams
 * mergeSort(, 0 ,markEnd, int oldPlacement, points) */
	free(newPlacement);
	free(newPosition);
	free(oldPlacement);
	free(teamThing);
	free(points);
	return SEASON_OK;
}

void merge(int arr[], int l, int m, int r, int oldPlacement, int* arr2)
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
		else if (L[i] = R[j])
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
void mergeSort(int arr[], int l, int r, int oldPlacement, int* arr2)
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

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

