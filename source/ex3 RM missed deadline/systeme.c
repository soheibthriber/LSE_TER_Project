#include "systeme.h"
#include<string.h>
#include<stdlib.h>
systeme * Create_system ( char id[], int nmbTasks){

systeme * self;

	self = (systeme *)malloc (sizeof (struct system_s));

	strcpy(self->Id,id);
	self -> numberOfTasks=nmbTasks;
	self -> current_time_unit=-1;
	self-> tasks = (task *)malloc (nmbTasks*sizeof (struct task_s));	

return self;
}


void  init_task ( systeme * self, char task_id[], int capacity , int period, int deadline, int start_time,int position){

strcpy(self -> tasks[position].Id,task_id);	
self -> tasks[position].Capacity = capacity;
self -> tasks[position].Period = period;
self -> tasks[position].Deadline = deadline;
self -> tasks[position].Start_time = start_time;

self -> tasks[position].Debut = -1;
self -> tasks[position].Fin = -1;
self -> tasks[position].Activation_time = -1;
self -> tasks[position].time_consumed = 0;

self -> tasks[position].checked=false;
}


void Task_activation(systeme *self,int time_unit,int position){
}

void Start_of_task_capacity(systeme * self,int time_unit,int position){
self -> tasks[position].Debut = time_unit;
self -> tasks[position].Activation_time = time_unit;
}

void Task_running(systeme *self, int time_unit,int position){

self -> current_time_unit=time_unit;
self -> tasks[position].time_consumed++;

}


void End_of_task_capacity(systeme *self, int time_unit,int position){
self -> tasks[position].Fin = time_unit;
}


void Reinit_task ( systeme * self, int position){
self -> tasks[position].Debut = -1;
self -> tasks[position].Fin = -1;
self -> tasks[position].Activation_time = -1;
self -> tasks[position].time_consumed = 0;
}

bool Check_activation(systeme *self,int position){
bool res=false;
//printf("task %s activation test: current_time %d  activation_time %d\n",self->tasks[position].Id,self -> current_time_unit,self -> tasks[position].Activation_time);
if ( ((self -> tasks[position].Activation_time < self -> current_time_unit) && (self -> tasks[position].Activation_time >= self -> current_time_unit - self -> tasks[position].Period + 1))) res=false;
else res=true;
self -> tasks[position].checked=true;
return res;
}

bool Check_deadline(systeme  *self, int position){
bool result=false;
printf("task %s	Deadline test		current_time %d\n",self->tasks[position].Id,self -> current_time_unit);
if (self -> tasks[position].time_consumed != self -> tasks[position].Capacity) result=true;
return result; 
}






