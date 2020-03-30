/**
 * Implementation of various scheduling algorithms.
 *
 * Round-robin scheduling with Priority
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *head = NULL;

// pointer to the struct containing the next task
struct node *tmp;

// counter variable represents how many tasks have the same priority
int samePriorityCount = 1;

Task *pickNextTask();

// add a new task to the list of tasks
void add(char *name, int priority, int burst)
{
  // first create the new task
  Task *newTask = (Task *)malloc(sizeof(Task));

  newTask->name = name;
  newTask->priority = priority;
  newTask->burst = burst;

  // insert the new task into the list of tasks
  insert(&head, newTask);
}

/**
 * Run the priority scheduler
 */
void schedule()
{
  Task *current;
  tmp = head;

  while (head != NULL)
  {
    current = pickNextTask();
    if (samePriorityCount > 1)
    {
      // if there are more than 1 task with same priority run as normal round robin schedule
      // however we need to insert the task at the tail if the burst value is greater than quantum
      // value
      if (current->burst > QUANTUM)
      {

        run(current, QUANTUM);

        current->burst -= QUANTUM;
        delete (&head, current);

        //insert the task at the end of the list
        struct node *node = malloc(sizeof(struct node));
        node->task = current;
        node->next = NULL;

        //head is empty, place node at the end of the list, which is the head
        if (head == NULL)
        {
          head = node;
        }

        //must traverse the list otherwise
        else
        {
          struct node *tmp = head;
          while (tmp->next != NULL)
            tmp = tmp->next;

          tmp->next = node;
        }
      }
      else
      {
        run(current, current->burst);

        current->burst = 0;

        printf("Task %s finished.\n", current->name);
        delete (&head, current);
      }
    }

    //no duplicates, so we run normal round robin
    else
    {
      run(current, current->burst);
      current->burst = 0;
      printf("Task %s finished.\n", current->name);
      delete (&head, current);
    }
  }
}

// picks the next task to run based off of highest priority
// while updasting global samePriorityCount variable
Task *pickNextTask()
{

  struct node *temp;
  Task *hp = head->task;
  temp = head->next;
  samePriorityCount = 1;

  while (temp != NULL)
  {
    if (temp->task->priority > hp->priority)
    {
      hp = temp->task;
      samePriorityCount = 1;
    }
    else if (temp->task->priority == hp->priority)
    {
      samePriorityCount++; //same priority, increment
    }
    temp = temp->next;
  }

  return hp;
}