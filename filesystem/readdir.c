#include <stdio.h>
#include <dirent.h>

int
main(int argc, char* args[]) {
  DIR *dp;
  struct dirent *dirp;

  if((dp = opendir(args[1])) == NULL) {
    return 1;
  }
  while((dirp = readdir(dp)) != NULL) {
    printf("%lld %s\n", dirp->d_ino, dirp->d_name);
  }
  closedir(dp);
  return 0;
}
