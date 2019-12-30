/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 


/*
void WorkPlan::display(bool verbose, bool testing)
{
	Task *traverse_day, *traverse_time;
	traverse_day = head;

	traverse_time = head;

	cout << head->day << ". DAY" << endl;

	while(traverse_day->next != head) {
		while(traverse_time) {
			cout << traverse_time->name << "  -  " << traverse_time->time << endl;
			traverse_time = traverse_time->counterpart;
		}
		traverse_day = traverse_day->next;
		traverse_time = traverse_day;

		cout << traverse_day->day << ". DAY" << endl;
	}
	traverse_time = traverse_day;
	while(traverse_time) {
		cout << traverse_time->name << "  -  " << traverse_time->time << endl;
		traverse_time = traverse_time->counterpart;
	}
}*/


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot =new Task;
		Task *compeer =new Task;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create()
{		
	head = NULL;
}

void WorkPlan::close()
{
	Task *traverse;
	Task *traverse_daily;
	
	head->previous->next = NULL; // Break cycle
	// So we will know when we are at the last node

	while(head) {

		traverse = head;
		head = head->next;

		while(traverse) { // Delete all tasks in a single day
			traverse_daily = traverse;
			traverse = traverse->counterpart;
			delete traverse_daily;
		}

	}

	head = NULL;
}












void WorkPlan::add(Task *task) 
{

	Task *newnode = new Task;
	newnode->name = new char;
	strcpy(newnode->name, task->name);
	newnode->day = task->day;
	newnode->time = task->time;
	newnode->priority = task->priority;

	if(head == NULL) { // First node is adding

		newnode->next = newnode;
		newnode->previous = newnode;
		newnode->counterpart = NULL;
		head = newnode;
		return;
	}


	else if(task->day < head->day) { // task's day is smaller then head's day

		newnode->counterpart = NULL;
		newnode->next = head;
		head->previous->next = newnode;
		newnode->previous = head->previous;
		head->previous = newnode;
		head = newnode;
		return;
	}

	Task *traverse = head;

	while(traverse->day < task->day) { // Loop to find day
		traverse = traverse->next;
		if(traverse == head) {
			break;
		}
	}

	if(traverse->day == task->day) { // Day found

		while(traverse->counterpart && traverse->counterpart->time < task->time) { // Loop to find time
			traverse = traverse->counterpart;
		}
		
		if(traverse->time == task->time) { // Ops :( First task of the day should be delayed
			checkAvailableNextTimesFor(task);
			int delay_day = getUsableDay();
			int delay_time = getUsableTime();

			newnode->day = delay_day;
			newnode->time = delay_time;

			if(traverse->priority < task->priority) { // Old task will be delayed
				// Hold data in temp
				int old_priority = traverse->priority;
				char *old_name = new char;
				strcpy(old_name, traverse->name);

				// Change data of current place
				strcpy(traverse->name, newnode->name);
				traverse->priority = newnode->priority;

				// Change data of newnode
				strcpy(newnode->name, old_name);
				newnode->priority = old_priority;
			
				delete old_name;
			}
	
			traverse = head;
			while(traverse->day != delay_day) { // Find day
				traverse = traverse->next;
			}

			if(traverse->time > delay_time) { // Task will be first task of day
				newnode->counterpart = traverse;
				newnode->next = traverse->next;
				newnode->previous = traverse->previous;
				traverse->previous->next = newnode;
				traverse->next->previous = newnode;
				traverse->next = NULL;
				traverse->previous = NULL;
				return;
			}

			while(traverse->counterpart) { // Find time
				if(traverse->counterpart->time < delay_time) {
					traverse = traverse->counterpart;
					continue;
				}
				break;
			}

			if(traverse->counterpart == NULL) { // Task will be last task of day
				traverse->counterpart = newnode;
				newnode->counterpart = NULL;
				return;
			}

			else { // Task will be in between counterparts
				newnode->counterpart = traverse->counterpart;
				traverse->counterpart = newnode;
				return;
			}

			return; 
		}

		else if(traverse->time > task->time) { // task will be first node of the day

			if(traverse == head) {
				if(head->next == head) {
					head = newnode;
					head->next = head;
					head->previous = head;
				}
				else {
					head->previous->next = newnode;
					head->next->previous = newnode;
					newnode->next = head->next;
					newnode->previous = head->previous;
					head = newnode;
				}
			}

			else {
				traverse->previous->next = newnode;
				traverse->next->previous = newnode;
				newnode->next = traverse->next;
				newnode->previous = traverse->previous;
			}

			newnode->counterpart = traverse;
			traverse->next = NULL;
			traverse->previous = NULL;
			return;
		}

		else if(traverse->counterpart == NULL && traverse->time < task->time) { // Task will be last node of the day

			traverse->counterpart = newnode;
			newnode->counterpart = NULL;
			return;
		}

		else if(traverse->counterpart->time > task->time) { // New task is after traverse before its counterpart

			newnode->counterpart = traverse->counterpart;
			traverse->counterpart = newnode;
			return;
		}

		else if(traverse->counterpart->time == task->time) { // Ops :( One task should be delayed
			traverse = traverse->counterpart;

			checkAvailableNextTimesFor(traverse);
			int delay_day = getUsableDay();
			int delay_time = getUsableTime();

			newnode->day = delay_day;
			newnode->time = delay_time;

			if(traverse->priority < task->priority) { // Old task will be delayed
				// Hold data in temp
				int old_priority = traverse->priority;
				char *old_name = new char;
				strcpy(old_name, traverse->name);

				// Change data of current place
				strcpy(traverse->name, newnode->name);
				traverse->priority = newnode->priority;

				// Change data of newnode
				strcpy(newnode->name, old_name);
				newnode->priority = old_priority;

				delete old_name;
			}

			traverse = head;
			while(traverse->day != delay_day) { // Find day
				traverse = traverse->next;
			}

			if(traverse->time > delay_time) { // Task will be first task of day
				newnode->counterpart = traverse;
				newnode->next = traverse->next;
				newnode->previous = traverse->previous;
				traverse->previous->next = newnode;
				traverse->next->previous = newnode;
				traverse->next = NULL;
				traverse->previous = NULL;
				return;
			}

			while(traverse->counterpart) { // Find time
				if(traverse->counterpart->time < delay_time) {
					traverse = traverse->counterpart;
					continue;
				}
				break;
			}

			if(traverse->counterpart == NULL) { // Task will be last task of day
				traverse->counterpart = newnode;
				newnode->counterpart = NULL;
				return;
			}

			else { // Task will be in between counterparts
				newnode->counterpart = traverse->counterpart;
				traverse->counterpart = newnode;
				return;
			}
		}

	} // Day found if block ends ----

	else if(traverse == head) { // task's day is larger than the largest day

		newnode->counterpart = NULL;
		head->previous->next = newnode;
		newnode->next = head;
		newnode->previous = head->previous;
		head->previous = newnode;
		return;
	}

	else if(task->day < traverse->day) { // No day exist but it will be in between

		newnode->counterpart = NULL;
		traverse->previous->next = newnode;
		newnode->previous = traverse->previous;
		traverse->previous = newnode;
		newnode->next = traverse;
		return;
	}

}


Task * WorkPlan::getTask(int day, int time)
{
	Task *traverse = head;

	while(traverse->day != day) { // Find day
		traverse = traverse->next;
	}

	while(traverse->time != time) { // Find time
		traverse = traverse->counterpart;
	}

	if(traverse->day == day && traverse->time == time) // If found
		return traverse;
	else
		return NULL;
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)
{
	Task *traverse_day = head;
	Task *traverse_time = NULL;

	while(traverse_day->day < delayed->day) { // Find day
		traverse_day = traverse_day->next;
	}
	traverse_time = traverse_day;

	while(traverse_time->day == delayed->day && traverse_time->counterpart && traverse_time->time < delayed->time) { // Find time
		traverse_time = traverse_time->counterpart;
	}

	while(traverse_time->counterpart) { // Check if available time is on the same day 
		if(traverse_time->counterpart->time - traverse_time->time > 1) {
			usable_day = traverse_time->day;
			usable_time = traverse_time->time + 1;
			return;
		}
		traverse_time = traverse_time->counterpart;
	}

	while(traverse_day->next != head) { // Go search for other days
		
		traverse_day = traverse_day->next;

		traverse_time = traverse_day;
		
		if(traverse_time->time > 8) { // Check if first task of that day is after 8
			usable_day = traverse_time->day;
			usable_time = 8;
			return;
		}

		while(traverse_time->counterpart) { // Check for available times on that day
			if(traverse_time->counterpart->time - traverse_time->time > 1) {
				usable_day = traverse_time->day;
				usable_time = traverse_time->time + 1;
				return;
			}
			traverse_time = traverse_time->counterpart;
		}
	}
	// No available time found

	traverse_day = head;

	while(traverse_day->day < delayed->day) { // Find day
		traverse_day = traverse_day->next;
	}

	traverse_time = traverse_day;

	while(traverse_time->counterpart) { // Go to end of the day
		traverse_time = traverse_time->counterpart;
	}
	
	if(traverse_time->time < 16) { // If possible delay to end of the same day
		usable_day = traverse_time->day;
		usable_time = traverse_time->time + 1;
		return;
	}

	while(traverse_day != head) { // Go search for other days
		
		traverse_day = traverse_day->next;

		traverse_time = traverse_day;

		while(traverse_time->counterpart) { // Check for available times on that day
			traverse_time = traverse_time->counterpart;
		}

		if(traverse_time->time < 16) { // If possible delay to end of that day
			usable_day = traverse_time->day;
			usable_time = traverse_time->time + 1;
			return;
		}
	}
}


void WorkPlan::delayAllTasksOfDay(int day)
{
	Task *traverse = head;
	Task *last_task = NULL;
	Task *tobe_delayed = NULL;
	Task *traverse_day = NULL;
	Task *traverse_time = NULL;

	int delay_day, delay_time;

	while(traverse->day != day) { // Find day
		traverse = traverse->next;
	}

	last_task = traverse;
	while(last_task->counterpart) { // Get last task so that every delay will be after that
		last_task = last_task->counterpart;
	}

	while(traverse) { // Delay each counterpart one by one

		tobe_delayed = traverse;

		if(tobe_delayed == head) {
			if(tobe_delayed->counterpart) {
				head = tobe_delayed->counterpart;
			}
			else {
				head = tobe_delayed->next;
			}
		}

		if(traverse->counterpart) { // Keep the list connected
			traverse->previous->next = traverse->counterpart;
			traverse->next->previous = traverse->counterpart;
			traverse->counterpart->next = traverse->next;
			traverse->counterpart->previous = traverse->previous;
		}
		else {
			traverse->previous->next = traverse->next;
			traverse->next->previous = traverse->previous;
		}
		traverse = traverse->counterpart;

		checkAvailableNextTimesFor(last_task);
		delay_day = getUsableDay();
		delay_time = getUsableTime();

		traverse_day = head;

		while(traverse_day->day < delay_day) { // Find day
			traverse_day = traverse_day->next;
		}

		traverse_time = traverse_day;

		if(traverse_time->time > delay_time) { // Task will be first task of day
			tobe_delayed->counterpart = traverse_time;
			traverse_time->previous->next = tobe_delayed;
			traverse_time->next->previous = tobe_delayed;
			tobe_delayed->next = traverse_time->next;
			tobe_delayed->previous = traverse_time->previous;
			tobe_delayed->day = delay_day;
			tobe_delayed->time = delay_time;
			continue;
		}

		while(traverse_time->counterpart) { // Find time
			if(traverse_time->counterpart->time < delay_time) {
				traverse_time = traverse_time->counterpart;
				continue;
			}
			break;
		}

		if(traverse_time->counterpart == NULL) { // Task will be last task of day
			tobe_delayed->day = delay_day;
			tobe_delayed->time = delay_time;
			traverse_time->counterpart = tobe_delayed;
			tobe_delayed->counterpart = NULL;
			continue;
		}

		else { // Task will be in between counterparts
			tobe_delayed->day = delay_day;
			tobe_delayed->time = delay_time;
			tobe_delayed->counterpart = traverse_time->counterpart;
			traverse_time->counterpart = tobe_delayed;
			continue;
		}
	}
}

void WorkPlan::remove(Task *target)
{
	if(target->next && target->previous) { // target is the first task of a day
		if(target->counterpart) {
			target->previous->next = target->counterpart;
			target->counterpart->previous = target->previous;
			target->counterpart->next = target->next;
			target->next->previous = target->counterpart;
		}
		else { 
			target->next->previous = target->previous;
			target->previous->next = target->next;
		}
	}


	else { // target is a counterpart
		Task *traverse = head;

		while(target->day != traverse->day) { // traverse becomes first task of target day
			traverse = traverse->next;
		}

		while(traverse->next != target) { // traverse becomes previous counterpart of target
			traverse = traverse->counterpart;
		}

		traverse->counterpart = target->counterpart;
	}

	if(target == head) { // Replace head if necessary
		if(head->counterpart) {
			head = head->counterpart;
		}
		else {
			head = head->next;
		}
	}

	delete target;
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
