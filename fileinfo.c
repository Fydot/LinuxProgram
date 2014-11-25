#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

char *getUsername(int uid)
{
	struct passwd *pw;
	pw = getpwuid(uid);
	return pw->pw_gecos;
}

char *getGroupname(int gid)
{
	struct group *gp;
	gp = getgrgid(gid);
	return gp->gr_name;
}

int main(int argc, char *args[])
{
	struct stat buf;
	stat(args[1], &buf);

	printf("Size: %d B\n", (int) buf.st_size);
	printf("Link: %d\n", buf.st_nlink);
	printf("Owner: %s\n", getUsername(buf.st_uid));
	printf("Group: %s\n", getGroupname(buf.st_gid));
	printf("Last Read: %s", ctime(&buf.st_atime));
	printf("Last Modify: %s", ctime(&buf.st_mtime));
	return 0;
}
