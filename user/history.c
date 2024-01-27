#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void
history()
{
  int fd;
  char buf[512];

  if((fd = open("/.sh_history", O_RDONLY)) < 0) {
    fprintf(2, "history: read error\n");
    exit(1);
  }

  int n;
  while((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      fprintf(2, "history: write error\n");
      exit(1);
    }
  }
  if(n < 0){
    fprintf(2, "history: read error\n");
    exit(1);
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  history();
  exit(0);
}