#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
  //size of shared memory object
  const int SIZE = 4096;
  //name of shared memory object
  const char *name = "OS";
  //message to write to shared memory
  const char *message = "Lab2 Practice";

  //shared memory file descriptor
  int shm_fd;
  //pointer to shared memory object
  void *ptr;

  //create shared memory object
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

  //configure size of shared memory object
  ftruncate(shm_fd, SIZE);

  //memory map the shared memory object
  ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

  //write to the shared memory object
  sprintf(ptr, "%s", message);
  printf("%s%s", "Wrote message to: ", name);
  ptr += strlen(message);
}