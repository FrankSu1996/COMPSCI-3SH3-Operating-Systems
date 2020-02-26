#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//function declarations for calculating average, minimum, and maximum
void *calcAverage(void *str);
void *calcMinimum(void *ptr);
void *calcMaximum(void *ptr);

//store average, min, max as global variables
//threads will update using calc functions as starting routine
double average;
int minimum;
int maximum;

typedef struct data_structure
{
  int size;
  int *values;
} data_structure;

int main(int argc, char *argv[])
{

  //user must enter at least 1 number
  if (argc <= 1)
  {
    printf("You must provide at least 1 number!\n");
    exit(1);
  }

  int i = 0;
  int temp[argc - 1];
  for (i; i < (argc - 1); i++)
  {
    temp[i] = atoi(argv[i + 1]);
  }

  pthread_t thread1, thread2, thread3;

  int td1, td2, td3;

  data_structure data_s = {argc - 1, temp};

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

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);

  printf("The average value is %g\n", average);
  printf("The minimum value is %d\n", minimum);
  printf("The maximum value is %d\n", maximum);

  exit(EXIT_SUCCESS);
}

void *calcAverage(void *ptr)
{
  data_structure *data;
  data = (data_structure *)ptr;

  int length = data->size;
  int i;

  for (i = 0; i < length; i++)
  {
    average += (data->values[i]);
  }
  average = (int)(average / length);
}

void *calcMinimum(void *ptr)
{
  data_structure *data;
  data = (data_structure *)ptr;

  int length = data->size;
  int i;

  minimum = (data->values[0]);
  for (i = 1; i < length; i++)
  {
    if (minimum > (data->values[i]))
    {
      minimum = (data->values[i]);
    }
  }
}

void *calcMaximum(void *ptr)
{
  data_structure *data;
  data = (data_structure *)ptr;

  int length = data->size;
  int i;

  maximum = data->values[0];

  for (i = 1; i < length; i++)
  {
    if (maximum < data->values[i])
    {
      maximum = data->values[i];
    }
  }
  return 0;
}