#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
#include<event.h>

int countnum=0;
void signal_handler(evutil_socket_t fd, short events, void *arg)
{
	struct event *ev=(struct event *)arg;
	printf("recive singal %d\n",fd);
	if(++countnum>5)
	{
		event_del(ev);
	}
}

int main()
{
	struct event_base *base=event_base_new();
	
	struct event ev;
	
	event_assign(&ev,base,SIGINT,EV_SIGNAL | EV_PERSIST,signal_handler,&ev);
	
	event_add(&ev,NULL);

	event_base_dispatch(base);

	event_base_free(base);
	return 0;
}
