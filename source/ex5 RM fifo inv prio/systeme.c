#include "systeme.h"
#include<string.h>
#include<stdlib.h>


systeme * Create_system ( char id[], int nmbTasks,int nmbRessource){

systeme * self;

	self = (systeme *)malloc (sizeof (struct system_s));

	strcpy(self->Id,id);
	self -> numberOfTasks=nmbTasks;
	self -> nmbOfRessources=nmbRessource;
	self -> current_time_unit=-1;
	self-> tasks = (task *)malloc (nmbTasks*sizeof (struct task_s));
	self-> ressources_tab = (ressource *)malloc (nmbRessource*sizeof (struct ressource_s));	
 	

return self;
}


void  init_task ( systeme * self, char task_id[],char name[] , int capacity ,int priority, int period, int deadline, int start_time,int position){

strcpy(self -> tasks[position].Id,task_id);	
strcpy(self -> tasks[position].Name,name);	
self -> tasks[position].Capacity = capacity;
self -> tasks[position].Period = period;
self -> tasks[position].Deadline = deadline;
self -> tasks[position].Start_time = start_time;
self -> tasks[position].Priority = priority;


self -> tasks[position].Activation_time = -1;
self -> tasks[position].time_consumed = 0;

self -> tasks[position].checked=false;

self -> tasks[position].nmb_res_needed=0;
self -> tasks[position].res_needed_tab=NULL;

}

void init_ressource(systeme * self,char id[],int nmbCritical_tasks,int res_position){ 
strcpy(self-> ressources_tab[res_position].res_Id,id); 
self-> ressources_tab[res_position].critical_task_tab = (critical_task *)malloc (nmbCritical_tasks*sizeof (critical_task));
self-> ressources_tab[res_position].nmbCritical_task=nmbCritical_tasks;
}

void  init_critical_task(systeme * self,char name[],int begin_t,int end_t ,int res_position,int critical_position){
strcpy(self-> ressources_tab[res_position].critical_task_tab[critical_position].name,name);
self-> ressources_tab[res_position].critical_task_tab[critical_position].begin=begin_t;
self-> ressources_tab[res_position].critical_task_tab[critical_position].end=end_t;
} 

void Task_activation(systeme *self,int time_unit,int position){
}

void Start_of_task_capacity(systeme * self,int time_unit,int position){
self -> tasks[position].Activation_time = time_unit;
}

void Task_running(systeme *self, int time_unit,int position){

self -> current_time_unit=time_unit;
self -> tasks[position].time_consumed++;

}


void End_of_task_capacity(systeme *self, int time_unit,int position){

}


void Reinit_task ( systeme * self, int position){

self -> tasks[position].Activation_time = -1;
self -> tasks[position].time_consumed = 0;
}

bool Check_activation(systeme *self,int position){
bool res=false;
printf("task %s activation test: current_time %d  activation_time %d\n",self->tasks[position].Id,self -> current_time_unit,self -> tasks[position].Activation_time);
if ( ((self -> tasks[position].Activation_time < self -> current_time_unit) && (self -> tasks[position].Activation_time >= self -> current_time_unit - self -> tasks[position].Period + 1))) res=false;
else res=true;
self -> tasks[position].checked=true;
return res;
}

bool Check_deadline(systeme  *self, int position){
bool result=false;
if (self -> tasks[position].time_consumed != self -> tasks[position].Capacity) result=true;
return result; 
}






