#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *calcAverage(void *str);
void *calcMinimum(void *ptr);
void *calcMaximum(void *ptr);

double average;
int minimum;
int maximum;

typedef struct data
{
  int size;
  int *values;
} data;

int main(int argc, char *argv[])
{

  int i = 0;
  int temp[argc - 1];
  for (i; i < (argc - 1); i++)
  {
    temp[i] = atoi(argv[i + 1]);
  }

  pthread_t thread1, thread2, thread3;
  const char *msg1 = "This is Thread 1";
  const char *msg2 = "This is Thread 2";
  const char *msg3 = "This is Thread 3";
  int td1, td2, td3;

  printf("Running: %s\n", argv[0]);

  data data_s = {argc - 1, temp};

  /* Create independent threadata_s each of which will execute appropriate function*/
  td1 = pthread_create(&thread1, NULL, (void *)calcAverage, (void *)&data_s);
  if (td1)
  {
    fprintf(stderr, "Error - pthread_create(): %d\n", td1);
    exit(EXIT_FAILURE);
  }

  td2 = pthread_create(&thread2, NULL, (void *)calcMinimum, (void *)&data_s);
  if (td2)
  {
    fprintf(stderr, "Error - pthread_create(): %d\n", td2);
    exit(EXIT_FAILURE);
  }

  td3 = pthread_create(&thread3, NULL, (void *)calcMaximum, (void *)&data_s);
  if (td3)
  {
    fprintf(stderr, "Error - pthread_create(): %d\n", td3);
    exit(EXIT_FAILURE);
  }

  printf("pthread_create() for Thread 1 : %d\n", td1);
  printf("pthread_create() for Thread 2 : %d\n", td2);
  printf("pthread_create() for Thread 3 : %d\n\n", td3);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);

  printf("The average: %g\n", average);
  printf("The minimum: %d\n", minimum);
  printf("The maximum: %d\n", maximum);

  exit(EXIT_SUCCESS);
}

void *calcAverage(void *ptr)
{
  data *temp;
  temp = (data *)ptr;

  int sz = temp->size;
  int i;

  for (i = 0; i < sz; i++)
  {
    average += (temp->values[i]);
  }
  average = (int)(average / sz);
}

void *calcMinimum(void *ptr)
{
  data *temp;
  temp = (data *)ptr;

  int sz = temp->size;
  int i;

  minimum = (temp->values[0]);
  for (i = 1; i < sz; i++)
  {
    if (minimum > (temp->values[i]))
    {
      minimum = (temp->values[i]);
    }
  }
}

void *calcMaximum(void *ptr)
{
  data *temp;
  temp = (data *)ptr;

  int sz = temp->size;
  int i;

  maximum = temp->values[0];

  for (i = 1; i < sz; i++)
  {
    if (maximum < temp->values[i])
    {
      maximum = temp->values[i];
    }
  }
  return 0;
}