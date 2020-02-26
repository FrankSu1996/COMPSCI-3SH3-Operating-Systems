#include <pthread.h>
#include <stdio.h>

//function declaration for printing out prime numbers
void printPrimes(int *);

int main(int argc, char *argv[])
{
  //user must enter a number
  if (argc <= 1)
  {
    printf("You must provide a number!\n");
    exit(1);
  }

  //store passed argument in variable
  int n = atoi(argv[1]);
  pthread_t thread1;

  //create thread1, pass printPrimes function as starting routine
  pthread_create(&thread1, NULL, (void *)printPrimes, (void *)&n);
  pthread_join(thread1, NULL);

  return 0;
}

//function to print all prime numbers less than or equal to input number
void printPrimes(int *n)
{
  printf("Prime numbers less than or equal to %d:\n", *n);

  //flag to indicate when prime number is found
  int foundPrimeFlag;
  int limit = *n;
  //loop from 1 to limit, check for primality
  for (int potentialPrime = 1; potentialPrime <= limit; potentialPrime++)
  {
    foundPrimeFlag = 0;
    //only need to check to potentialPrime/2
    for (int i = 2; i <= potentialPrime / 2; ++i)
    {
      // condition for prime number
      if (potentialPrime % i == 0)
      {
        foundPrimeFlag = 1;
        break;
      }
    }

    //print out prime number
    if (foundPrimeFlag == 0)
      printf("%d ", potentialPrime);
  }
  printf("\n");
}