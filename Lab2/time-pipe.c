#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>

#define BUFFER_SIZE 2096
#define READ_END 0
#define WRITE_END 1

//alias for timeval struct
typedef struct timeval timeval_t;

int main(int argc, char *argv[])
{

  //Pipe creation
  int fd[2];
  if (pipe(fd) == -1)
  {
    fprintf(stderr, "Pipe failed");
  }

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
    //close unused end of the pipe
    close(fd[READ_END]);
    //save time of day before command
    timeval_t startTime;
    gettimeofday(&startTime, NULL);
    //write to pipe and close it
    write(fd[WRITE_END], startTime);
    close(fd[WRITE_END]);
    //execute command specified from command line
    execvp(argv[1], argv + 1);
  }
  //parent process
  else
  {
    wait(NULL);
    //get the end time
    timeval_t endTime;
    gettimeofday(&endTime, NULL);
    //get the start time from shared memory
    timeval_t startTime;
    close(fd[WRITE_END]);
    read(fd[READ_END], startTime);
    //calculate elapsed time
    timeval_t elapsedTime;
    timersub(&endTime, &startTime, &elapsedTime);
    //print elapsed time
    printf("\nElapsed time: %d.%06d seconds\n", elapsedTime.tv_sec, elapsedTime.tv_usec);
  }
}