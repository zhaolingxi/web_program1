#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<stdlib.h>
#include<event.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{

	int fd = open("fifo.temp", O_WRONLY);
	if (-1 == fd)
	{
		perror("open");
		exit(1);
	}

	char buf[32] = { 0 };
	while (1)
	{
		scanf("%s", buf);
		int ret = write(fd, buf, strlen(buf));
		if (-1 == ret)
		{
			perror("write");
			exit(1);
		}
		if (!strcmp(buf, "bye"))
		{
			break;
		}
		memset(buf, 0, sizeof(buf));
	}
	return 0;
}
