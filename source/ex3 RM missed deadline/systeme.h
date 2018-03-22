#ifndef _TASK_H
#define _TASK_H
#include <stddef.h>
#include <stdbool.h>
  
typedef struct task_s {

char Id[5] ; int Capacity ;int Period; int Deadline; int Start_time; int  Priority; int Debut; int  Fin;  int Activation_time; int time_consumed;  
bool checked; 

}task; 
 


  
typedef struct system_s {
char Id[5]; int numberOfTasks ; struct task_s * tasks; int current_time_unit; 
  }systeme; 
 

systeme * Create_system ( char id[], int nmbTasks);


void init_task ( systeme * self, char task_id[], int capacity , int period, int deadline, int start_time,int position);

void Task_activation(systeme *self,int time,int position);

void Start_of_task_capacity(systeme * self,int time_unit,int position);

void Task_running(systeme  *self, int time_unit,int position);

void End_of_task_capacity(systeme *self, int time_unit,int position);

void Reinit_task ( systeme * self, int position);  


bool Check_activation(systeme *self,int position );
 
bool Check_deadline(systeme  *self, int position);


#endif 
