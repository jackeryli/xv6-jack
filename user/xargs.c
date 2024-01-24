#include "kernel/types.h"
#include "user/user.h"

int
getcmd(char *buf, int nbuf)
{
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

int
main(int argc, char *argv[])
{
  char *args[argc+1];
  int i;
  char buf[512];

  for(i=1; i<argc; i++)
    args[i-1] = argv[i];

  while(getcmd(buf, sizeof(buf)) >= 0){
    buf[strlen(buf)-1] = 0; // chop \n
    args[argc-1] = buf;
    args[argc] = 0;

    if(fork() == 0){
      exec(args[0], args);
      printf("xargs: exec error\n");
      exit(1);
    } else {
      wait(0);
    }
  }

  exit(0);
}
