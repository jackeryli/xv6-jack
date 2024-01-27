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

  int count = 1;
  int n;
  int atLineStart = 1;

  while((n = read(fd, buf, sizeof(buf))) > 0) {
    for (int i = 0; i < n; i++) {
      if (atLineStart) {
        fprintf(1, "%d ", count);
        atLineStart = 0; // Reset the flag
        count++;
      }

      if (buf[i] == '\n') {
        atLineStart = 1; // Set the flag for the next line
      }

      if (write(1, &buf[i], 1) != 1) {
        fprintf(2, "history: write error\n");
        exit(1);
      }
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