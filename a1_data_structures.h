/********* a1_data_structures.h ********
	
	Student Name 	= Emma Wong 
	Student Number	= 101297761
*/
#include <stdbool.h>

#ifndef __a1_data_structures_H__
#define __a1_data_structures_H__

#define NUM_ACTIVITIES 3
#define ABC_SALARY 30.0
/********** DON'T MODIFY FROM HERE **********/

typedef struct activity {
    unsigned short int id;
    char name[100];
    float planned_cost, actual_cost;
    unsigned short int planned_duration, actual_duration;
    _Bool completed;
}activity_t;

typedef struct milestone {
    unsigned short int id;
    char name[100];
    activity_t *activity_list;
    _Bool completed;
    float actual_cost;
    short int actual_duration;
}milestone_t;

typedef struct project {
    char name[100]; 
    float planned_cost, actual_cost;
    unsigned short int planned_duration, actual_duration;
    _Bool completed;
}project_t;

/********** DON'T MODIFY UNTIL HERE **********/
#endif