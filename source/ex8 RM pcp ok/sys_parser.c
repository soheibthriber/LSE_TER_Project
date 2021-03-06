/*

this code source is wrriten in the context of an academic project in the master LSE (ubo 2018)
the aim of this project is to test a numbre of real time scheduling properties by analysing a trace generated by a real time systeme running on a FPGA. 

this trace is provided in an xml format, and it's devided into 2 files : 
the first provides the information of the real time system( nmb of proc/core, nmb and properties of the periodique tasks, and idem for the shared ressources. 
and the second represent the actual trace with the different types of events that occured on the system.

Our code contains 3 source file : 

-----> systeme.h and systeme.c : where the ADT (Abstract Data Type) is defined ( struct and functions). 

-----> sys_parser.c : the main sourcefile that parse the xml files of any given examples and analyse the RT properties.

the over all scheme of the process goes like this: 

§ 1-  we first begin by parsing the xml file that represent the RT system and create a temporary array of stucures (tasks and ressources prop).

§ 2- then we use those arrays and the constructure functions of the ADT to allocate space and initialise our instance of the systeme ADT) . after that we free the tmp structures 
     
§ 3- then we proceed to the parsing of the xml file (the actual trace) and create another temporary array of a structutre event ( describe all the occuring events) 

% 4- finnaly we cross this array element by element and excute the analyse algorithme .     
  
*/
#include <libxml/parser.h>
#include <libxml/tree.h>
#include<stdio.h>
#include<string.h>
#include "systeme.h"

typedef struct task_r {

char Id[6] ; char name[3]; int Capacity ;int Period; int Deadline; int Start_time; int  Priority; bool res_need; char * char_res_needed;      
}task_r;


typedef struct crtical_task_r{
char name[3]; int begin ; int end; 
}critical_task_r;

typedef struct ressource_r{
char res_Id[6]; critical_task_r * critical_task_tab; int nmbCritical_task;
}ressource_r;


typedef struct event_s{
char Op; int time_unit;char task_selected[6] ; char res_selected[6];     
}event;

int nmb_Res=0,pos_Res=-1; 
int nmb_Crit=0,pos_Crit=-1; 

char tmp_Res_Id[6];
char tmp_Task_Name[3];

int tmp_task_begin,tmp_task_end;

int task_position=-1;

critical_task_r * tmp_crit_tab=NULL;
ressource_r * tmp_res_tab=NULL;

task_r * tmp_task_tab=NULL;

xmlDoc*  doc  = NULL;

systeme * exo1;

char core_id[6];
char tmp_task_id[6]; 
char tmp_name[3];
int nmbT=0;
int tmp_capacity=0,tmp_period=0,tmp_deadline=0,tmp_start_time=0,tmp_priority=0;
int nmbEvents=0,tmp_time_unit;
char tmp_Op,tmp_task_selected[6];

event * event_tab= NULL;



void get_critical_sections( xmlNode* root){

	xmlChar *key;
	xmlNode* current = NULL;	

	for ( current=root; current!=NULL; current=current->next )
		{		
               
                        if ( strcmp((char*)current->name,"task_name")==0 )
				{
				 nmb_Crit++,pos_Crit++; 
				strcpy(tmp_Task_Name,xmlNodeListGetString(doc, current->xmlChildrenNode, 1));
				}

			if ( strcmp((char*)current->name,"task_begin")==0 )
				{
				key = xmlNodeListGetString(doc, current->xmlChildrenNode, 1);
				tmp_task_begin=atoi(key);	
				xmlFree(key);
				}
			if ( strcmp((char*)current->name,"task_end")==0 )
				{
				key = xmlNodeListGetString(doc, current->xmlChildrenNode, 1);
				tmp_task_end=atoi(key);
				xmlFree(key);

				tmp_crit_tab=(critical_task_r*) realloc(tmp_crit_tab,nmb_Crit*sizeof(critical_task_r));
				if(tmp_crit_tab==NULL){printf("ERR"); return;}
				strcpy(tmp_crit_tab[pos_Crit].name,tmp_Task_Name);
				tmp_crit_tab[pos_Crit].begin=tmp_task_begin;tmp_crit_tab[pos_Crit].end=tmp_task_end;			
				}
		get_critical_sections(current->children); 
		}
}

	

void get_ressources( xmlNode* root){

xmlChar *key;
	xmlNode* current = NULL;

int i;

for ( current=root; current!=NULL; current=current->next )
		{
		
			if ( strcmp((char*)current->name,"pcp_resource")==0 )	 
				{
				nmb_Res++;pos_Res++;
				strcpy(tmp_Res_Id,(const char*)xmlGetProp( current, (const xmlChar*)"id") );		
				}		
         
			if ( strcmp((char*)current->name,"critical_sections")==0 ) {nmb_Crit=0,pos_Crit=-1;get_critical_sections(current->children );}		
			

			if ( strcmp((char*)current->name,"priority_assignment")==0 )
				{int i=0;
 				tmp_res_tab=(ressource_r*) realloc(tmp_res_tab,nmb_Res*sizeof(ressource_r));
				if(tmp_res_tab==NULL){printf("ERR"); return;}
				strcpy(tmp_res_tab[pos_Res].res_Id,tmp_Res_Id);
				tmp_res_tab[pos_Res].nmbCritical_task=nmb_Crit;
				tmp_res_tab[pos_Res].critical_task_tab=(critical_task_r*) malloc(nmb_Crit*sizeof(critical_task_r));
				for(i=0;i<nmb_Crit;i++){
				strcpy(tmp_res_tab[pos_Res].critical_task_tab[i].name,tmp_crit_tab[i].name);
				tmp_res_tab[pos_Res].critical_task_tab[i].begin=tmp_crit_tab[i].begin;
				tmp_res_tab[pos_Res].critical_task_tab[i].end=tmp_crit_tab[i].end;
							}				
				tmp_crit_tab=NULL;
 				}		
		get_ressources(current->children); 
		}
}


void get_tasks( xmlNode* root){

xmlChar *key;
	xmlNode* current = NULL;

	for ( current=root; current!=NULL; current=current->next )
		{
		
			if ( strcmp((char*)current->name,"periodic_task")==0 )	 
				{
				task_position++;nmbT++;
				strcpy(tmp_task_id,(const char*)xmlGetProp( current, (const xmlChar*)"id") );	
				}		

			if ( strcmp((char*)current->name,"name")==0 )
				{
				strcpy(tmp_name,xmlNodeListGetString(doc, current->xmlChildrenNode, 1) );	
				}

			if ( strcmp((char*)current->name,"capacity")==0 )
				{
				key = xmlNodeListGetString(doc, current->xmlChildrenNode, 1);
				tmp_capacity=atoi(key);				
				xmlFree(key);
				}
			if ( strcmp((char*)current->name,"deadline")==0 )
				{
				key = xmlNodeListGetString(doc, current->xmlChildrenNode, 1);
				tmp_deadline=atoi(key);
				xmlFree(key);
				}
			if ( strcmp((char*)current->name,"period")==0 )
				{
				key = xmlNodeListGetString(doc, current->xmlChildrenNode, 1);
				tmp_period=atoi(key);
				xmlFree(key);
					}
			if ( strcmp((char*)current->name,"start_time")==0 )
				{
				key = xmlNodeListGetString(doc, current->xmlChildrenNode, 1);
				tmp_start_time=atoi(key);				
				xmlFree(key);
				}
			if ( strcmp((char*)current->name,"priority")==0 )
				{
				key = xmlNodeListGetString(doc, current->xmlChildrenNode, 1);
				tmp_priority=atoi(key);				
				xmlFree(key);
				}
			if ( strcmp((char*)current->name,"every")==0 )
				{
 				tmp_task_tab=(task_r*) realloc(tmp_task_tab,nmbT*sizeof(task_r));

				if(tmp_task_tab==NULL){
					printf("ERROR");
					return;
				}
				strcpy(tmp_task_tab[task_position].Id,tmp_task_id);
				strcpy(tmp_task_tab[task_position].name,tmp_name);
				tmp_task_tab[task_position].Capacity=tmp_capacity;tmp_task_tab[task_position].Deadline=tmp_deadline;
				tmp_task_tab[task_position].Period=tmp_period;tmp_task_tab[task_position].Start_time=tmp_start_time;
				tmp_task_tab[task_position].Priority=tmp_priority;	
 				}
		get_tasks(current->children); 
		}
}



void get_content( xmlNode* root )
{
	xmlChar *key;
	xmlNode* current = NULL;

	for ( current=root; current!=NULL; current=current->next )
		{
  
		if ( strcmp((char*)current->name,"core_unit")==0 ) 
		{
		strcpy(core_id,(const char*)xmlGetProp( current, (const xmlChar*)"id") );
		} 
		
		if ( strcmp((char*)current->name,"tasks")==0 ) { get_tasks(current->children ); } 
		if ( strcmp((char*)current->name,"resources")==0 ) { get_ressources(current->children );}		
		
		get_content( current->children );
		}
}



void get_events( xmlNode* root )
{
	xmlChar *key;
	xmlNode* current = NULL;

	for ( current=root; current!=NULL; current=current->next )
		{
		
		
			if ( strcmp((char*)current->name,"time_unit")==0 )	 
				{
				nmbEvents++;
				key = xmlNodeListGetString(doc, current->xmlChildrenNode, 1);
				tmp_time_unit=atoi(key);				
				xmlFree(key);
				}
			

				if ( strcmp((char*)current->name,"activation_task")==0 )	 
				{
				strcpy(tmp_task_selected,(const char*)xmlGetProp( current, (const xmlChar*)"ref" ) );	
				tmp_Op='A';
				event_tab= (event *) realloc(event_tab,nmbEvents*sizeof(event));
					if(event_tab==NULL){
						printf("ERROR");
					}
				strcpy(event_tab[nmbEvents-1].task_selected,tmp_task_selected);
				event_tab[nmbEvents-1].Op=tmp_Op;event_tab[nmbEvents-1].time_unit=tmp_time_unit;
				}			
				
				
				if ( strcmp((char*)current->name,"start_task")==0 )	 
				{
				strcpy(tmp_task_selected,(const char*)xmlGetProp( current, (const xmlChar*)"ref" ) );	
				tmp_Op='S';
				event_tab= (event *) realloc(event_tab,nmbEvents*sizeof(event));
					if(event_tab==NULL){
						printf("ERROR");
					}
				strcpy(event_tab[nmbEvents-1].task_selected,tmp_task_selected);
				event_tab[nmbEvents-1].Op=tmp_Op;event_tab[nmbEvents-1].time_unit=tmp_time_unit;
				}			
				

				if ( strcmp((char*)current->name,"running_task")==0 )	 
				{
				strcpy(tmp_task_selected,(const char*)xmlGetProp( current, (const xmlChar*)"ref" ) );	
				tmp_Op='R';
				event_tab= (event *) realloc(event_tab,nmbEvents*sizeof(event));
					if(event_tab==NULL){
						printf("ERROR");
					}
				strcpy(event_tab[nmbEvents-1].task_selected,tmp_task_selected);
				event_tab[nmbEvents-1].Op=tmp_Op;event_tab[nmbEvents-1].time_unit=tmp_time_unit;
				}
				
				
				if ( strcmp((char*)current->name,"end_task")==0 )	 
				{
				strcpy(tmp_task_selected,(const char*)xmlGetProp( current, (const xmlChar*)"ref" ) );	
				tmp_Op='E';
				event_tab= (event *) realloc(event_tab,nmbEvents*sizeof(event));
					if(event_tab==NULL){
						printf("ERROR");
					}
				strcpy(event_tab[nmbEvents-1].task_selected,tmp_task_selected);
				event_tab[nmbEvents-1].Op=tmp_Op;event_tab[nmbEvents-1].time_unit=tmp_time_unit;
				}
				
				
				if ( strcmp((char*)current->name,"allocate_task")==0 )	 
				{
				strcpy(tmp_task_selected,(const char*)xmlGetProp( current, (const xmlChar*)"ref" ) );	
				tmp_Op='C';
				event_tab= (event *) realloc(event_tab,nmbEvents*sizeof(event));
					if(event_tab==NULL){
						printf("ERROR");
				}	
				strcpy(event_tab[nmbEvents-1].task_selected,tmp_task_selected);
				event_tab[nmbEvents-1].Op=tmp_Op;event_tab[nmbEvents-1].time_unit=tmp_time_unit;
				}
				if ( strcmp((char*)current->name,"allocate_resource")==0 )	 
				{		
				strcpy(event_tab[nmbEvents-1].res_selected,(const char*)xmlGetProp( current, (const xmlChar*)"ref"));
				}

				
				if ( strcmp((char*)current->name,"release_task")==0 )	 
				{
				strcpy(tmp_task_selected,(const char*)xmlGetProp( current, (const xmlChar*)"ref" ) );	
				tmp_Op='L';
				event_tab= (event *) realloc(event_tab,nmbEvents*sizeof(event));
					if(event_tab==NULL){
						printf("ERROR");
				}	
				strcpy(event_tab[nmbEvents-1].task_selected,tmp_task_selected);
				event_tab[nmbEvents-1].Op=tmp_Op;event_tab[nmbEvents-1].time_unit=tmp_time_unit;
				}
				if ( strcmp((char*)current->name,"release_resource")==0 )	 
				{		
				strcpy(event_tab[nmbEvents-1].res_selected,(const char*)xmlGetProp( current, (const xmlChar*)"ref"));
				}
				

				if ( strcmp((char*)current->name,"wait_for_resource_task")==0 )	 
				{
				strcpy(tmp_task_selected,(const char*)xmlGetProp( current, (const xmlChar*)"ref" ) );	
				tmp_Op='W';
				event_tab= (event *) realloc(event_tab,nmbEvents*sizeof(event));
					if(event_tab==NULL){
						printf("ERROR");
				}	
				strcpy(event_tab[nmbEvents-1].task_selected,tmp_task_selected);
				event_tab[nmbEvents-1].Op=tmp_Op;event_tab[nmbEvents-1].time_unit=tmp_time_unit;
				}
				if ( strcmp((char*)current->name,"wait_for_resource")==0 )	 
				{		
				strcpy(event_tab[nmbEvents-1].res_selected,(const char*)xmlGetProp( current, (const xmlChar*)"ref"));

				}
		get_events( current->children );
		}
}


void get_trace( xmlNode* root )
{
	xmlChar *key;
	xmlNode* current = NULL;

	for ( current=root; current!=NULL; current=current->next )
		{
		
		if ( strcmp((char*)current->name,"result")==0 ) { get_events(current->children ); break;} 

		get_trace( current->children );
		}
}




int main()
{
	int i,task_position,j,l;
	doc = xmlReadFile( "exo5_2tuto.xml", NULL, 0 );
	xmlNode* root = NULL;


	if ( doc==NULL ) return 0;
   
	root = xmlDocGetRootElement( doc );
	get_content( root );
	xmlFreeDoc( doc );
	xmlCleanupParser();
   
	char tasks_map[nmbT][6];
	char task_name_map[nmbT][3];
	exo1 = Create_system(core_id,nmbT,nmb_Res);

	for( i=0; i<nmbT; i++)
		{
		strcpy(tasks_map[i],tmp_task_tab[i].Id );  // storing the Ids of the tasks in order to obtain the position 	
		strcpy(task_name_map[i],tmp_task_tab[i].name);		
		init_task(exo1,tmp_task_tab[i].Id,tmp_task_tab[i].name,tmp_task_tab[i].Capacity,tmp_task_tab[i].Priority,tmp_task_tab[i].Period,tmp_task_tab[i].Deadline,tmp_task_tab[i].Start_time,i);
		}
	free(tmp_task_tab);	

	for( i=0; i<nmb_Res; i++)
		{
		init_ressource(exo1,tmp_res_tab[i].res_Id, tmp_res_tab[i].nmbCritical_task,i);
			for( j=0; j<exo1->ressources_tab[i].nmbCritical_task; j++)
			{
			init_critical_task(exo1,tmp_res_tab[i].critical_task_tab[j].name,tmp_res_tab[i].critical_task_tab[j].begin,tmp_res_tab[i].critical_task_tab[j].end,i,j);
			}
		}

	free(tmp_res_tab);




for( i=0; i<exo1 ->numberOfTasks ; i++)
		{
		//printf("task id is %s and name %s and prio %d \n",exo1->tasks[i].Id,exo1->tasks[i].Name,exo1->tasks[i].Priority);
		}
//*********************************************************************************

for(i=0;i< exo1->nmbOfRessources;i++){

for( j=0; j<exo1->ressources_tab[i].nmbCritical_task; j++)
		{
		for(l=0;l<nmbT;l++) {if (strcmp(exo1->ressources_tab[i].critical_task_tab[j].name,task_name_map[l])==0)	task_position=l;}
               
		exo1->tasks[task_position].nmb_res_needed++;
		exo1->tasks[task_position].res_needed_tab= (res_needed*)realloc(exo1->tasks[task_position].res_needed_tab, exo1->tasks[task_position].nmb_res_needed*sizeof(res_needed));
		strcpy(exo1->tasks[task_position].res_needed_tab[exo1->tasks[task_position].nmb_res_needed-1].res_needed_Id,exo1->ressources_tab[i].res_Id);
		exo1->tasks[task_position].res_needed_tab[exo1->tasks[task_position].nmb_res_needed-1].begin_need=exo1->ressources_tab[i].critical_task_tab[j].begin;
		exo1->tasks[task_position].res_needed_tab[exo1->tasks[task_position].nmb_res_needed-1].end_need=exo1->ressources_tab[i].critical_task_tab[j].end;


		//printf("%s Crit name is s %s b %d  e %d pos %d \n",exo1->ressources_tab[i].res_Id,exo1->ressources_tab[i].critical_task_tab[j].name,exo1->ressources_tab[i].critical_task_tab[j].begin,exo1->ressources_tab[i].critical_task_tab[j].end,task_position);
		}
}
	



for(i=0;i< nmbT;i++){

for( j=0; j<exo1->tasks[i].nmb_res_needed; j++)
		{
		//printf("task %s  needs resource %s from %d  to %d \n",exo1->tasks[i].Id,exo1->tasks[i].res_needed_tab[j].res_needed_Id,exo1->tasks[i].res_needed_tab[j].begin_need,exo1->tasks[i].res_needed_tab[j].end_need);
		}
}
	

//**********************************************************************************
	
	doc = xmlReadFile( "trace_exo5_2tuto.xml", NULL, 0 );
	root = NULL;
	if ( doc==NULL ) return 0;
	root = xmlDocGetRootElement( doc );
	get_trace(root);	
	xmlFreeDoc( doc );
	xmlCleanupParser();
       





for (i=0;i<nmbEvents;i++)
	{
	int j;
	for(j=0;j<nmbT;j++)	{	if (strcmp(event_tab[i].task_selected,tasks_map[j])==0)	task_position=j;}



	for(j=0;j < exo1-> numberOfTasks;j++){ 
		if ( (exo1 -> current_time_unit % exo1 -> tasks[j].Period == exo1 -> tasks[j].Period - 1) && (! exo1 -> tasks[j].checked) )
		{
		if(Check_activation(exo1,j)) printf ("?!?!?!?!?no activation alert task %s ?!?!?!?!?\n",tasks_map[j]);
	        if(Check_deadline(exo1,j)) printf ("?!?!?!?!?deadline alert task %s !?!?!?!?!?!?!?!?\n",tasks_map[j]);
		Reinit_task ( exo1, j);			
		}
		
		   
	}
	for(j=0;j < exo1-> numberOfTasks;j++){ 	
	if ( exo1 -> current_time_unit % exo1 -> tasks[j].Period == 0 ) {exo1 -> tasks[j].checked=false;} //printf("task %s checked state %d\n",tasks_map[j],exo1 -> tasks[j].checked);}
		}


      
	switch(event_tab[i].Op) {
		case 'A':
			Task_activation(exo1,event_tab[i].time_unit,task_position);
		break;


		case 'S':
			Start_of_task_capacity(exo1,event_tab[i].time_unit, task_position);
		break;


		case 'R':
			Task_running(exo1,event_tab[i].time_unit,task_position); 
		//printf("time consummed %d **** activation_time %d \n",exo1 -> tasks[task_position].time_consumed,exo1 -> tasks[task_position].Activation_time);  
		break;

		case 'E':
			End_of_task_capacity(exo1,event_tab[i].time_unit,task_position);
		break;

		case 'C':
			allocate_resource(exo1,event_tab[i].time_unit,task_position,event_tab[i].res_selected);
		break;

		case 'L':
			release_resource(exo1,event_tab[i].time_unit,task_position,event_tab[i].res_selected);
		break;
	
		case 'W':
			wait_for_resource(exo1,event_tab[i].time_unit,task_position,event_tab[i].res_selected);
		break;

		default:
			printf("no valid operation\n");
		break;
		}	
}		
return 1;
}
