#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netdb.h>
#define TCP_PROTO "tcp"
int proxy_port;
struct sockaddr_in hostaddr;
extern int errno;
extern char *sys_myerrlist[];
void parse_args(int argc, char **argv);
void daemonize(int servfd);
void do_proxy(int usersockfd);
void reap_status(void);
void errorout(char *msg);
typedef void Signal(int);

int main(int argc, char *argv[])
{
	int clilen;
	int childpid;
	int sockfd, newsockfd;
	struct sockaddr_in servaddr, cliaddr;
	parse_args(argc, argv);
	bzero((char *) &servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = proxy_port;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fputs("failed to create server socket\r\n", stderr);
		return 1;
	}
	if (bind
	    (sockfd, (struct sockaddr_in *) &servaddr,
	     sizeof(servaddr)) < 0) {
		fputs("faild to bind server socket to specified port\r\n",
		      stderr);
		return 1;
	}
	listen(sockfd, 5);
	daemonize(sockfd);
	while (1) {
		clilen = sizeof(cliaddr);
		newsockfd =
		    accept(sockfd, (struct sockaddr_in *) &cliaddr,
			   &clilen);
		if (newsockfd < 0 && errno == EINTR)
			continue;
		else if (newsockfd < 0)
			errorout("failed to accept connection");
		if ((childpid = fork()) == 0) {
			close(sockfd);
			do_proxy(newsockfd);
			return 1;
		}
		lose(newsockfd);
	}
	return 0;
}


void parse_args(argc, argv)
int argc;
char **argv;
{
	int i;
	struct hostent *hostp;
	struct servent *servp;
	unsigned long inaddr;
	struct {
		char proxy_port[16];
		char isolated_host[64];
		char service_name[32];
	} pargs;
	strcpy(pargs.proxy_port, argv[1]);
	strcpy(pargs.isolated_host, argv[2]);
	strcpy(pargs.service_name, argv[3]);
	for (i = 0; i < strlen(pargs.proxy_port); i++)
		if (!isdigit(*(pargs.proxy_port + i)))
			break;
	　　if (i == strlen(pargs.proxy_port))
		proxy_port = htons(atoi(pargs.proxy_port));
	　　
	else {
		printf("%s: invalid proxy port\r\n", pargs.proxy_port);
		exit(0);
	}
	bzero(&hostaddr, sizeof(hostaddr));
	hostaddr.sin_family = AF_INET;
	if ((inaddr = inet_addr(pargs.isolated_host)) != INADDR_NONE)
		bcopy(&inaddr, &hostaddr.sin_addr, sizeof(inaddr));
	else if ((hostp = gethostbyname(pargs.isolated_host)) != NULL)
		bcopy(hostp->h_addr, &hostaddr.sin_addr, hostp->h_length);
	else {
		printf("%s: unknown host\r\n", pargs.isolated_host);
		exit(1);
	}
	if ((servp = getservbyname(pargs.service_name, TCP_PROTO)) != NULL)
		hostaddr.sin_port = servp->s_port;
	else if (atoi(pargs.service_name) > 0)
		hostaddr.sin_port = htons(atoi(pargs.service_name));
	else {
		printf
		    ("%s: invalid/unknown service name or port number\r\n",
		     pargs.service_name);
		exit(1);
	}
}


void daemonize(servfd)
int servfd;
{
	int childpid, fd, fdtablesize;
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	if ((childpid = fork()) < 0) {
		fputs("failed to fork first child\r\n", stderr);
		exit(1);
	　　　}
	　　
	else if (childpid > 0)
		　　　exit(0);	/* terminate parent, continue in child */
	　　　		/* dissociate from process group */
	    　　if (setpgrp(0, getpid()) < 0) {
		　　　　fputs
		    ("failed to become process group leader\r\n", stderr);
		　　　　exit(1);
	　　}
	　　			/* lose controlling terminal */
	    　　if ((fd = open("/dev/tty", O_RDWR)) >= 0) {
		　　　　ioctl(fd, TIOCNOTTY, NULL);
		　　　　close(fd);
	　　}
	　　			/* close any open file descriptors */
	    　　for (fd = 0, fdtablesize = getdtablesize();
		       fd < fdtablesize; fd++)
		　　if (fd != servfd)
			　　　close(fd);
	　　　		/* set working directory to allow filesystems to be unmounted */
	    　　　chdir("/");
	　　　		/* clear the inherited umask */
	    　　　umask(0);
	　　　		/* setup zombie prevention */
	    　　　signal(SIGCLD, (Sigfunc *) reap_status);
　　}


void reap_status()
{
	　　int pid;
	　　union wait status;
	　　while ((pid = wait3(&status, WNOHANG, NULL)) > 0)
		　　;		/* loop while there are more dead children */
}


void do_proxy(usersockfd)
int usersockfd;
{
	　　int isosockfd;
	　　fd_set rdfdset;
	　　int connstat;
	　　int iolen;
	　　char buf[2048];
	　　			/* open a socket to connect to the isolated host */
	    　　if ((isosockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		　　errorout("failed to create socket to host");
	　　			/* attempt a connection */
	    　　connstat =
	    connect(isosockfd, (struct sockaddr *) &hostaddr,
		    sizeof(hostaddr));
	　　switch (connstat) {
	　　case 0:
		　　break;
	　　case ETIMEDOUT:
	　　case ECONNREFUSED:
	　　case ENETUNREACH:
		　　strcpy(buf, sys_myerrlist[errno]);
		　　strcat(buf, "\r\n");
		　　write(usersockfd, buf, strlen(buf));
		　　close(usersockfd);
		　　exit(1);
		　　		/* die peacefully if we can't establish a connection */
		    　　break;
	　　default:
		　　errorout("failed to connect to host");
	　　}
	　　			/* now we're connected, serve fall into the data echo loop */
	    　　while (1) {
		　　　　	/* Select for readability on either of our two sockets */
		    　　　　FD_ZERO(&rdfdset);
		　　　　FD_SET(usersockfd, &rdfdset);
		　　　　FD_SET(isosockfd, &rdfdset);
		　　if (select(FD_SETSIZE, &rdfdset, NULL, NULL, NULL) <
			  0)
			　　　errorout("select failed");
		　　　	/* is the client sending data? */
		    　　if (FD_ISSET(usersockfd, &rdfdset)) {
			　　　　if ((iolen =
					 read(usersockfd, buf,
					      sizeof(buf))) <= 0)
				　　　　　break;	/* zero length means the client disconnected */
			　　　　　rite(isosockfd, buf, iolen);
			　　　　　	/* copy to host -- blocking semantics */
		　　　}
		　　		/* is the host sending data? */
		    　　if (FD_ISSET(isosockfd, &rdfdset)) {
			　　　　f((iolen =
				       read(isosockfd, buf,
					    sizeof(buf))) <= 0)
			    � ��　　　　　break;	/* zero length means the host disconnected */
			　　　　　　rite(usersockfd, buf, iolen);
			　　　　　　	/* copy to client -- blocking semantics */
		　　　　}
	　　　}
	　　　		/* we're done with the sockets */
	    　　　close(isosockfd);
	　　　lose(usersockfd);
　　}
