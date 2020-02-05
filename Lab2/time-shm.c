#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>

//alias for timeval struct
typedef struct timeval timeval_t;
//size of shared memory object
const int SIZE = 4096;
//name of shared memory object
const char *name = "SHM";

int main(int argc, char *argv[])
{
  //size of shared memory object
  const int SIZE = 4096;
  //name of shared memory object
  const char *name = "OS";
  //shared memory file descriptor
  int shm_fd;
  //create shared memory object
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  //configure size of shared memory object
  ftruncate(shm_fd, SIZE);
  //memory map the shared memory object
  timeval_t *sharedMemory = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

  //fork a child process
  pid_t pid;
  pid = fork();

  //error occured
  if (pid < 0)
  {
    fprintf(stderr, "Fork failed");
    return 1;
  }
  //child process
  else if (pid == 0)
  {
    //save time of day to shared memory
    gettimeofday(sharedMemory, NULL);
    //execute command specified from command line
    execvp(argv[1], argv + 1, NULL);
  }
  //parent process
  else
  {
    wait(NULL);
    //get the end time
    timeval_t endTime;
    gettimeofday(&endTime, NULL);
    //get the start time from shared memory
    timeval_t startTime = *sharedMemory;
    //calculate elapsed time
    timeval_t elapsedTime;
    timersub(&endTime, &startTime, &elapsedTime);
    //print elapsed time
    printf("\nElapsed time: %d.%06d seconds\n", elapsedTime.tv_sec, elapsedTime.tv_usec);
  }
}