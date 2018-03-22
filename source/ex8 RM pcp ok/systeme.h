/*the system header file 
defining the structures and the functions*/

#ifndef _TASK_H
#define _TASK_H
#include <stddef.h>
#include <stdbool.h>

//structure that represent a ressource needed by a task   
typedef struct res_needed_s {
char res_needed_Id[6];int begin_need;int end_need; 
}res_needed;

 //struct represent task  
typedef struct task_s {
// static properties of a task      
char Id[6] ;char Name[3]; int Capacity ;int Period; int Deadline; int Start_time; int  Priority;
// varibles that represent the state  
int Activation_time; int time_consumed;bool checked; 
// number and a table of the ressource needed 
int nmb_res_needed;res_needed * res_needed_tab; 
// number and array of  ressource identifiars that the task wait for 
int nmb_res_waiting;
char **waiting_res;
// number and array of ressource identifiars that the task is using now 
int nmb_res_using; 
char **using_res;   

}task; 
 

// structure that represent the critical section (tasks) of a given ressource (used by the ressource_s structure)
typedef struct crtical_task_s{
char name[3]; int begin ; int end; 
}critical_task;

// struct that reresente the task
typedef struct ressource_s{
char res_Id[6]; critical_task * critical_task_tab; int nmbCritical_task; 
}ressource;

// struct that represent the system  
typedef struct system_s {
char Id[6]; int numberOfTasks ; struct task_s * tasks; int nmbOfRessources;struct ressource_s * ressources_tab ;int current_time_unit; 
}systeme; 
 
 

systeme * Create_system ( char id[], int nmbTasks,int nmbRessource);


void  init_task ( systeme * self, char task_id[],char name[] , int capacity ,int priority, int period, int deadline, int start_time,int position);

void init_ressource(systeme * self,char id[],int nmbCritcal_tasks,int res_position);

void init_critical_task(systeme * self,char name[],int begin_t,int end_t ,int res_position,int critical_position);

void Task_activation(systeme *self,int time,int position);

void Start_of_task_capacity(systeme * self,int time_unit,int position);

void Task_running(systeme  *self, int time_unit,int position);

void End_of_task_capacity(systeme *self, int time_unit,int position);

void Reinit_task ( systeme * self, int position);  

void allocate_resource ( systeme * self,int time_unit, int task_position,char  res_id[6]);
void release_resource (	 systeme * self,int time_unit,int task_position,char  res_id[6]);
void wait_for_resource ( systeme * self,int time_unit,int task_position,char  res_id[6]);

bool Check_activation(systeme *self,int position );
 
bool Check_deadline(systeme  *self, int position);


#endif 
