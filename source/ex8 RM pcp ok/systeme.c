/* the systeme c file: developping the abstract data types functions  functions*/


#include "systeme.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
int nmb_Matchs=0;

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
self -> tasks[position].waiting_res=NULL;
self -> tasks[position].using_res=NULL;
self -> tasks[position].nmb_res_using=0;
self -> tasks[position].nmb_res_waiting=0;
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
//printf("task %s  current_time %d time consummed %d\n",self->tasks[position].Id,self -> current_time_unit,self -> tasks[position].time_consumed);
}


void End_of_task_capacity(systeme *self, int time_unit,int position){

}


void Reinit_task ( systeme * self, int position){

self -> tasks[position].Activation_time = -1;
self -> tasks[position].time_consumed = 0;
}



void allocate_resource ( systeme * self,int time_unit, int task_position,char  res_id[6]){
int  i,resource_position;

printf("current time_unit:%d		",time_unit);
self->tasks[task_position].nmb_res_using++;

self->tasks[task_position].using_res = realloc(self->tasks[task_position].using_res, self->tasks[task_position].nmb_res_using * sizeof(char *));


for (i = self->tasks[task_position].nmb_res_using-1 ; i < self->tasks[task_position].nmb_res_using; i++) {
    self->tasks[task_position].using_res[i] = (char *)malloc(6*sizeof(char));
}

self->tasks[task_position].using_res[self->tasks[task_position].nmb_res_using-1]=res_id;

for (i = 0; i < self->tasks[task_position].nmb_res_using; i++) {
printf("task %s now using %s	",self->tasks[task_position].Id,self->tasks[task_position].using_res[i]);
}
printf("\n");

//***************************************
if(self->tasks[task_position].waiting_res>0){
self->tasks[task_position].nmb_res_waiting--; 

if(self->tasks[task_position].nmb_res_waiting==0){free(self->tasks[task_position].waiting_res);}

for (i = 0; i < self->tasks[task_position].nmb_res_waiting; i++) {
if(strcmp(res_id,self->tasks[task_position].waiting_res[i])==0) self->tasks[task_position].waiting_res[i][0]='\0';
}	


for (i = 0; i < self->tasks[task_position].nmb_res_waiting; i++) {
printf(" task %s now waiting %s	",self->tasks[task_position].Id,self->tasks[task_position].waiting_res[i]);
}
printf("\n");
}

//********************************



for(i=0;i<self->tasks[task_position].nmb_res_needed;i++) {if (strcmp(res_id,self->tasks[task_position].res_needed_tab[i].res_needed_Id)==0)	resource_position=i;}

if(self -> tasks[task_position].time_consumed == (self -> tasks[task_position].res_needed_tab[resource_position].begin_need - 1)) { 
//printf("task %s  allocate resource %s in the expected time\n",self -> tasks[task_position].Id,self->tasks[task_position].res_needed_tab[resource_position].res_needed_Id);
}
else {printf("!!!!??????????allocation error!!!!!!!!!!!????????\n");}

}


void release_resource ( systeme * self,int time_unit ,int task_position,char  res_id[6]){
int  i,resource_position;

printf("current time_unit:%d		",time_unit);

if(self->tasks[task_position].using_res>0){

self->tasks[task_position].nmb_res_using--; 

if(self->tasks[task_position].nmb_res_using==0){free(self->tasks[task_position].using_res);}

for (i = 0; i < self->tasks[task_position].nmb_res_using; i++) {
if(strcmp(res_id,self->tasks[task_position].using_res[i])==0) self->tasks[task_position].using_res[i][0]='\0';
}	


for (i = 0; i < self->tasks[task_position].nmb_res_using; i++) {
printf("task %s now using %s	",self->tasks[task_position].Id,self->tasks[task_position].using_res[i]);
}
printf("\n");
}


for(i=0;i<self->tasks[task_position].nmb_res_needed;i++) {if (strcmp(res_id,self->tasks[task_position].res_needed_tab[i].res_needed_Id)==0)	resource_position=i;}

if(self -> tasks[task_position].time_consumed == (self -> tasks[task_position].res_needed_tab[resource_position].end_need - 1)) { 
//printf("task %s  release resource %s in the expected time\n",self -> tasks[task_position].Id,self->tasks[task_position].res_needed_tab[resource_position].res_needed_Id);
}
else {printf("!!!!??????????release error!!!!!!!!!!!????????\n");}
}


void wait_for_resource ( systeme * self, int time_unit, int task_position,char  res_id[6]){
int i , j , k ,resource_position;


if(self->tasks[task_position].nmb_res_waiting>0) {if(strcmp(res_id,self->tasks[task_position].waiting_res[0])==0) return;}
printf("current time_unit:%d		",time_unit);
self->tasks[task_position].nmb_res_waiting++;

self->tasks[task_position].waiting_res = realloc(self->tasks[task_position].waiting_res, self->tasks[task_position].nmb_res_waiting * sizeof(char *));


for (i = self->tasks[task_position].nmb_res_waiting-1 ; i < self->tasks[task_position].nmb_res_waiting; i++) {
    self->tasks[task_position].waiting_res[i] = (char *)malloc(6*sizeof(char));
}

self->tasks[task_position].waiting_res[self->tasks[task_position].nmb_res_waiting-1]=res_id;

for (i = 0; i < self->tasks[task_position].nmb_res_waiting; i++) 
	{

	for (j=0; j < self-> numberOfTasks; j++)
		{ 
		for (k=0; k < self->tasks[j].nmb_res_using; k++)
			{ 
  			if(strcmp(self->tasks[task_position].waiting_res[i], self->tasks[j].using_res[k])==0)
				{
				nmb_Matchs++;
				if ( nmb_Matchs==2) printf("!!!!!!!!!??????????????interblockage task %s and task %s !!!!!!!!!!!!!!!!!!!!!!!!!!???????????????????????\n",self->tasks[task_position].Id,self->tasks[j].Id);
				}		 			
			}

	}  

printf("task %s now waiting for %s	",self->tasks[task_position].Id ,self->tasks[task_position].waiting_res[i]);
}
printf("\n");


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
if (self -> tasks[position].time_consumed != self -> tasks[position].Capacity) result=true;
return result; 
}






