#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
pwd()
{
  char *path = ".";
  char buf[512];
  int fd;
  struct dirent de;
  struct stat st, parentst;
  char *result = "";

  if((fd = open(path, 0)) < 0){
    fprintf(2, "pwd: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "pwd: cannot stat %s\n", path);
    close(fd);
    return;
  }

  strcpy(buf, path);
  
  int maxdepth = 20;
  int depth = 0;

  for(; depth < maxdepth; depth++){

    // Go to parent directory
    strcpy(buf + strlen(buf), "/..");
    
    if((fd = open(buf, 0)) < 0){
      fprintf(2, "pwd: cannot open %s\n", buf);
      return;
    }

    if(fstat(fd, &parentst) < 0){
      fprintf(2, "pwd: cannot stat %s\n", buf);
      close(fd);
      return;
    }

    // If parent ino is equal to current ino,
    // it indicates that it reaches the root
    if(parentst.ino == st.ino)
      break;

    // Iterate parent directory and find the folder
    // that match current directory by checking ino
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if(de.inum == st.ino){
        char resultbuf[512];
        strcpy(resultbuf, result);
        strcpy(result, "/");
        strcpy(result+1, de.name);
        strcpy(result+strlen(result), resultbuf);
      }
      
    }
    // Move to parent folder
    st = parentst;
  }

  // Root folder
  if(strlen(result) == 0)
    result = "/";

  printf("%s\n", result);
  close(fd);
}

int
main(int argc, char *argv[])
{
  pwd();
  exit(0);
}