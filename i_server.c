#include<stdio.h>
#include<stdlib.h>
#include<event.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<string.h>
#include<event2/listener.h>

void read_cb(struct bufferevent *bev, void *ctx)
{
	int fd=*(int *)ctx;
	char * buf[128]={0}; 
	size_t ret=bufferevent_read(bev,buf,sizeof(buf));
	if(ret<0)
	{
		printf("read_fail\n");
		exit(1);
	}
	else
	{
		printf("read form: %d %s\n",fd,buf);
	}
}

void event_cb(struct bufferevent *bev, short what, void *ctx)
{
	if(what &BEV_EVENT_EOF)
	{
		int fd1=*(int*)ctx; 
		printf("BEV_EVENT_EOF %d \n",fd1);
		bufferevent_free(bev);
	}
	else   {
                printf("error\n");
        }
}

void listenercb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int socklen, void *arg)
{
	printf("connect with:",fd);
	
	struct event_base* base=arg;

	struct bufferevent * BV=bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
	
	if(!BV)
	{
		printf("BV fail\n");
		exit(1);
	}

	bufferevent_setcb(BV, read_cb,NULL,event_cb,&fd);

	bufferevent_enable(BV,EV_READ);
}


int main()
{
	struct event_base* base=event_base_new();
	if(!base)
	{
		printf("constrct fail\n");
		exit(1);
	}
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=8000;
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

	struct evconnlistener *listener= evconnlistener_new_bind(base,listenercb,NULL,LEV_OPT_CLOSE_ON_FREE |LEV_OPT_REUSEABLE,10,
			(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(!listener)
	{
		printf("construct listener fail\n");
		exit(1);
	}

	event_base_dispatch(base);
	
	evconnlistener_free(listener);

	event_base_free(base);

	return 0;
}
