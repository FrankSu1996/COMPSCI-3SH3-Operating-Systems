/**
 * Implementation of various scheduling algorithms.
 *
 * SJF scheduling
 */

#include <stdlib.h>
#include <stdio.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

// reference to the head of the list
struct node *head = NULL;

// sequence counter of next available thread identifier
int nextTid = 0;

Task *pickNextTask();

// add a new task to the list of tasks
void add(char *name, int priority, int burst)
{
  // first create the new task
  Task *newTask = (Task *)malloc(sizeof(Task));

  newTask->name = name;
  newTask->tid = nextTid++;
  newTask->priority = priority;
  newTask->burst = burst;

  // insert the new task into the list of tasks
  insert(&head, newTask);
}

/**
 * Run the FCFS scheduler
 */
void schedule()
{
  Task *current;

  // sanity checker
  traverse(head);

  while (head != NULL)
  {
    current = pickNextTask();

    run(current, current->burst);

    delete (&head, current);
  }
}

/**
 * Returns the next task selected to run.
 */
Task *pickNextTask()
{
  struct node *temp;
  Task *hp = head->task;
  temp = head->next;

  // variable to store next shortest job to run
  int shortestJob = hp->burst;

  while (temp != NULL)
  {
    // linearly traverse list to find shortest burst time
    if (temp->task->burst < shortestJob)
    {
      hp = temp->task;
      shortestJob = temp->task->burst;
    }

    temp = temp->next;
  }

  return hp;
}
