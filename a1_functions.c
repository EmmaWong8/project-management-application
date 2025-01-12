/********* a1_functions.c ********
	
	Student Name 	= Emma Wong 
	Student Number	= 101297761
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "a1_functions.h"

#define MAX_HOURS 8

unsigned short int get_input_usi(void){
	int input;
	unsigned short int valid_input;
	bool valid = false;

	while (valid==false){
		printf("-> Enter a positive integer:");
		scanf("%d", &input);

		if (input>0 && input<65536){
			valid_input = (unsigned short int)input;
			valid = true;
		} else {
			printf("INVALID INPUT. Try again.\n");
		}
	}
	return valid_input;
}

float get_input_f(void){
	float input;
	bool valid = false;

	while (valid==false){
		printf("-> Enter a positive float:");
		scanf("%f", &input);

		if (input>0){
			valid = true;
		} else {
			printf("ERROR: Value must be positive. Try again.");
		}
	}

	return input;
}

bool check_ID(unsigned short int id, unsigned short int *used_IDs, int num_used_IDs){
	for (int i=0; i<num_used_IDs; i++){
		if (used_IDs[i] == id){
			return true; //ID is already used
		}
	}

	return false; //ID is not used yet
}

void init_activity(activity_t * activity_to_int){
	float planned_duration = 0.0;

	printf("Enter a name for the activity with ID(%d):", activity_to_int->id);
	scanf("%s", activity_to_int->name);

	printf("Enter planned duration in hours:\n");
	planned_duration = get_input_f();

	//Calculate planned cost
	activity_to_int->planned_cost = planned_duration*ABC_SALARY;

	//Initialize other fields
	activity_to_int->actual_cost = 0;
	activity_to_int->planned_duration = planned_duration;
	activity_to_int->actual_duration = 0;
	activity_to_int->completed = false;

}

void init_milestone(milestone_t * milestone_to_int, unsigned short int num_activities){

	printf("Enter a name for the milestone:");
	scanf("%s", milestone_to_int->name);

	//Initialize each activity
	for (unsigned short int i=0; i<num_activities; i++){
		init_activity(&milestone_to_int->activity_list[i]);
	}

	//Initialize other fields
	milestone_to_int->completed = false;
	milestone_to_int->actual_cost = 0;
	milestone_to_int->actual_duration = 0;
}

project_t init_project(char name[], milestone_t *milestone_list, int number_milestones, const int * number_activities){
	project_t proj;

	//Initialize project name
	for (int i=0; name[i] != '\0';i++){
		proj.name[i] = name[i];
	}

	//Initialize project fields
	proj.planned_cost = 0.0;
	proj.actual_cost = 0.0;
	proj.planned_duration = 0.0;
	proj.actual_duration = 0.0;
	proj.completed = false;

	int sum_planned_duration =0;

	//Go through milestrones and their activities to calculate total
	for (int i=0; i<number_milestones; i++){
		for (int j=0; j<*number_activities; j++){

			//Sum of planned cost from activities
			proj.planned_cost += milestone_list[i].activity_list[j].planned_cost;

			//Sum of planned duration
			sum_planned_duration += (int)(milestone_list[i].activity_list[j].planned_duration);
		}
	}

	proj.planned_duration = round(sum_planned_duration/MAX_HOURS);
	return proj;
}

void print_main_menu(void){
	printf("\n*****MAIN MENU*****\n");
	printf("Please select one of the following options:\n");
    printf("1. Update activity\n");
    printf("2. Print stats\n");
    printf("3. Exit\n");
}

void print_milestone_stats(const milestone_t * list_milestones, int num_milestones, const int * number_activities){
	for (int i=0; i<num_milestones; i++){
		const milestone_t *milestone = &list_milestones[i];
		printf("\n---> MILESTONE ID(%d)\n", milestone->id);

		//Calculate planned cost and plammed duration of milestone
		float planned_cost = 0.0;
		int planned_duration = 0;
        for (int j = 0; j < number_activities[i]; j++) {
            planned_cost += milestone->activity_list[j].planned_cost;
			planned_duration += milestone->activity_list[j].planned_duration;
        }
		planned_duration = round(planned_duration/MAX_HOURS);

		if (milestone->completed){
			//Completed milestones
			printf("Name: %s\n", milestone->name);
			printf("Cost of Miletone: $ %.2f %s budget\n", fabs(planned_cost - milestone->actual_cost), milestone->actual_cost > planned_cost ? "over" : "under");
			printf("Duration of Milestone: %d days %s\n", abs(planned_duration - milestone->actual_duration), milestone->actual_duration > planned_duration ? "delayed" : "early");
		} else {
			//Incompleted milestones
			printf("Name: %s\n", milestone->name);
			int completed_activities =0;
			for (int j=0; j<number_activities[i]; j++){
				const activity_t *activity = &milestone->activity_list[j];
				if (activity->completed){
					completed_activities +=1;
				}
			}
			printf("Number of completed activities: %d\n",completed_activities);
			printf("Planned duration: %d day(s)\n", planned_duration);
			printf("Planned cost: $ %.2f\n",planned_cost);
		}
	}
}

void print_project_stats(project_t details, const milestone_t * list_milestones, int num_milestones, const int * number_activities){
	printf("\n*****PROJECT STATS*****\n");
  	printf("Project Name: %s\n", details.name);
    printf("Completed: %s\n", details.completed ? "Yes" : "No");

	if(details.completed){
		printf("Cost of Project: $ %.2f %s budget\n", fabs(details.planned_cost - details.actual_cost), details.actual_cost > details.planned_cost ? "over" : "under");
		printf("Duration of Project: %d day(s) %s\n",abs(details.planned_duration - details.actual_duration),  details.actual_duration > details.planned_duration ? "delayed" : "early");

	} else {
		print_milestone_stats(list_milestones, num_milestones, number_activities);
	}
}

void print_submenu(const milestone_t * milestone, int num_activities){
    for (int i = 0; i < num_activities; i++) {
        const activity_t *activity = &milestone->activity_list[i];
        printf("-> Activity ID: %d, Name: %s, Completed: %s\n", activity->id, activity->name, activity->completed ? "Yes" : "No");
    }
}

void update_activity(activity_t * activity_to_update){
	
	printf("\n*****UPDATING ACTIVITY with ID(%d)*****\n",activity_to_update->id);

    //Update the actual duration
	float actual_duration;
    printf("Enter actual duration in hours:\n");
    actual_duration = get_input_usi();
    activity_to_update->actual_duration = actual_duration;

	float actual_cost;
	printf("Enter actual cost in dollars:\n");
	actual_cost = get_input_f();
	activity_to_update->actual_cost = actual_cost;

    //Update completion status
    char completion_choice;
    printf("Is the activity completed? (y/n): ");
    scanf(" %c", &completion_choice);
    activity_to_update->completed = (completion_choice == 'y' || completion_choice == 'Y');
}

void update_milestone(milestone_t * milestone_to_update, int activities_in_milestone){
	
	float total_actual_cost = 0.0;
    unsigned short int total_actual_duration = 0;
    bool all_completed = true;

    for (int i = 0; i < activities_in_milestone; i++) {
        total_actual_cost += milestone_to_update->activity_list[i].actual_cost;
        total_actual_duration += milestone_to_update->activity_list[i].actual_duration;
        
		//Check if activity is complete
        if (!milestone_to_update->activity_list[i].completed) {
            all_completed = false;
        }
    }

    //Update milestone 
    milestone_to_update->actual_cost = total_actual_cost;
    milestone_to_update->actual_duration = total_actual_duration;
    milestone_to_update->completed = all_completed;
}

void update_project(const milestone_t * milestone_array, int num_milestones, const int * number_activities, project_t * my_project){
    float total_actual_cost = 0.0;
    unsigned short int total_actual_duration = 0;
    bool all_completed = true;

    for (int i = 0; i < num_milestones; i++) {
        total_actual_cost += milestone_array[i].actual_cost;
        total_actual_duration += milestone_array[i].actual_duration;

        //Check if milestone is complete
        if (!milestone_array[i].completed) {
            all_completed = false;
        }
    }

    //Update project 
    my_project->actual_cost = total_actual_cost;
    my_project->actual_duration = round(total_actual_duration/MAX_HOURS);
    my_project->completed = all_completed;
}