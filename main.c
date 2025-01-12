/********* main.c ********
	
	Student Name 	= Emma Wong
	Student Number	= 101297761
*/

//Complete the includes
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "a1_functions.h"

int main(){
    /** 1-  Display a welcome message **/
    
    printf("Welcome to ABC Company's project management application!\n");

    /** 2- Get the project's name and number of milestones **/
    char project_name[100];
    unsigned short int num_milestones = 0;

    printf("Please enter the project name:");
    scanf("%s",project_name);

    printf("Enter the number of milestones:\n");
    num_milestones = get_input_usi();

    //Dynamically allocate memory for milestones
    milestone_t *milestones = malloc(num_milestones * sizeof(milestone_t));
    assert(milestones != NULL);
    
    /** 3- Initialize all milestones and activities.
     * You need an array of milestones. 
     * The capacity of the array must be the max number of milestones the program 
     * can handle as per the instructions **/

    unsigned short int *used_milestone_IDs = malloc(num_milestones * sizeof(unsigned short int));
    assert(used_milestone_IDs != NULL);

    unsigned short int milestone_count = 0;

    unsigned short int *used_activity_IDs = malloc(num_milestones * sizeof(unsigned short int));
    assert(used_activity_IDs != NULL);
    
	unsigned short int activity_count =0;

    int *num_activities_array = malloc(num_milestones * sizeof(int));
    assert(num_activities_array != NULL);

    //Initialize milestones
    for(unsigned short int i=0; i<num_milestones; i++){
        printf("\n***** MILESTONE %d *****\n",i+1);

        //Check ID
        unsigned short int milestone_ID;
        printf("Enter a milestone ID:\n");
        milestone_ID = get_input_usi();

        while(check_ID(milestone_ID, used_milestone_IDs, milestone_count)){
            printf("ERROR: Milestone ID is already used. Try again.\n");
            printf("Enter a milestone ID:\n");
            milestone_ID = get_input_usi();
        }

        //Save milestone ID
        used_milestone_IDs[milestone_count++] = milestone_ID;
        milestones[i].id = milestone_ID;

        //Get number of activities for this milestone
        unsigned short int num_activities =0;
        printf("Enter the number of activities for this milestone:\n");
        num_activities = get_input_usi();

        num_activities_array[i] = num_activities;   //Save the number of activities 

        //Dynamically allocate memory for each milestones's activities
        milestones[i].activity_list = malloc(num_activities * sizeof(activity_t));
        assert(milestones[i].activity_list != NULL);

        //Initialize each activity
        for (unsigned short int j = 0; j < num_activities; j++) {

            unsigned short int activity_ID;
            printf("Enter an activity ID for activity %d:\n", j+1);
            activity_ID = get_input_usi();

            while(check_ID(activity_ID, used_activity_IDs, activity_count)){
                printf("ERROR: Activiy ID is already used. Try again.\n");
                printf("Enter an activity ID for activity %d:\n", j+1);
                activity_ID = get_input_usi();
            }

            //Save activity ID
            used_activity_IDs[activity_count++] = activity_ID;
            milestones[i].activity_list[j].id = activity_ID;
        }

        //Initialize milestone's activities  
        init_milestone(&milestones[i], num_activities);

        printf("Milestone %d initialization complete.\n", i+1);
    }

    /** 4- Initialize the project **/
    project_t project = init_project(project_name, milestones, num_milestones, num_activities_array);

    /** 5- Print the main menu **/
    print_main_menu();
   
    /** 6- Get user choice and handle it as per the instructions**/
    int choice; // Variable to store user choice
    bool running = true; // Control variable for the main loop

    while (running) {

        // Get user choice
        printf("Enter your choice (1-3): ");
        scanf("%d", &choice);

        // Handle user choice
        switch (choice) {
            case 1:
                //print sub menu
                printf("\n*****ACTIVITY LIST*****\n");
                
                bool uncompleted_activity_exists = false;

                //Check if there is at least one activity to update
                for(int i = 0; i < num_milestones && !uncompleted_activity_exists; i++){
                    for (int j = 0; j < num_activities_array[i]; j++) {
                        if (!milestones[i].activity_list[j].completed) {
                            uncompleted_activity_exists = true;
                            break;
                        }
                    }
                }

                if(!uncompleted_activity_exists){
                    printf("No activities availiable to update. Returning to the main menu.\n");
                    print_main_menu();
                    break;
                }

                //Print submenu
                for (int i = 0; i < num_milestones; i++) {
                    print_submenu(&milestones[i], num_activities_array[i]);
                }

                // Get user choice for activity to update
                unsigned short int activity_choice;
                printf("Enter the Activity ID to update: ");
                scanf("%d", &activity_choice);

                bool activity_found = false;
                for (int i = 0; i < num_milestones; i++) {
                    for (int j = 0; j < num_activities_array[i]; j++) {
                        if (milestones[i].activity_list[j].id == activity_choice && !milestones[i].activity_list[j].completed) {
                            activity_found = true;

                            update_activity(&milestones[i].activity_list[j]);   // Update the activity
                            update_milestone(&milestones[i], num_activities_array[i]);  // Update milestone
                            update_project(milestones, num_milestones, num_activities_array, &project);    // Update project
                            
                            printf("Activity, milestone, and project updated successfully.\n");
                            break;
                        }
                    }
                    if (activity_found){
                    break;
                    }
                }

                if (!activity_found) {
                    printf("ERROR: Invalid choice. Activity ID not found or already completed.\n");
                }
                print_main_menu();
                break;
            case 2:
                //print project status
                print_project_stats(project, milestones, num_milestones, num_activities_array);
                print_main_menu();
                break;
            case 3:
                //end program
                running = false;
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("ERROR: Invalid choice. Please select a valid option (1-3).\n");
                print_main_menu();
                break;
        }
        
    }

    //Free dynamically allocated memory
    for(int i=0; i<num_milestones;i++){
        free(milestones[i].activity_list);
    }
    free(milestones);
    free(used_milestone_IDs);
    free(used_activity_IDs);
    free(num_activities_array);

    return EXIT_SUCCESS;
}