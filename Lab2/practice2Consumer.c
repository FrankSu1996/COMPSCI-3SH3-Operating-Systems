#include <stdio.h>
#include <stdlib.h>
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
  shm_fd = shm_open(name, O_RDONLY, 0666);

  //memory map the shared memory object
  ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

  printf("%s%s", "Message read from shared memory: ", (char *)ptr);

  //remote the shared memory object
  shm_unlink(name);
}