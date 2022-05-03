#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<stdlib.h>
#include<event.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
void fifo_read(evutil_socket_t fd, short events, void * arg)
{
	char buf[32]={0};
	int ret=read(fd,buf ,sizeof(buf));
	if(-1==ret)
        {
                perror("read");
                exit(1);
        }
	printf("read form pipe %s\n",buf);
}
int main()
{
	int ret=mkfifo("fifo.temp",00700);
	if(-1==ret)
	{
		perror("mkfifo");
		exit(1);
	}

	int fd=open("fifo.temp",O_RDONLY);
	if(-1==fd)
        {
                perror("open");
                exit(1);
        }
	
	struct event ev;
	
	//init
	event_init();
	
	event_set(&ev,fd,EV_READ | EV_PERSIST,fifo_read,NULL);
	
	event_add(&ev,NULL);

	event_dispatch();
	return 0;
}
