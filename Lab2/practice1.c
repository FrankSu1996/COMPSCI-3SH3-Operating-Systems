#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
  pid_t pid;

  //fork a child process
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
    execlp("/bin/ls", "ls", NULL);
    printf("Hello from child process");
  }
  //parent process
  else
  {
    printf("Hello from parent process");
    //parent wait for the child to complete
    wait(NULL);
    printf("Child Complete");
  }
}