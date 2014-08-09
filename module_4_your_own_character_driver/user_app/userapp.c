#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/Gil_device"


int main(int argc, const char *argv[])
{
	int fd;
	char ch, write_buf[100], read_buf[100];

	fd = open(DEVICE,O_RDWR);

	if (fd == -1)
	{
		printf("\nfile %s either does not exist or has been locked by another process \n",DEVICE);
		exit(-1);
	}
	
	printf("\nr = read from device\nw = write into device\n");
	scanf("%c",&ch);
	
	switch(ch)
	{
		case 'w':
					printf("\nEnter data: \n");
					//scanf("%[^\n]\n",write_buf);
					scanf("%s",write_buf);
					write(fd,write_buf,sizeof(write_buf));
	
		case 'r':
					read(fd,read_buf,sizeof(read_buf));
					printf("device %s\n",read_buf);
					break;
		default:
					printf("\nCommand not recognized\n");
					break;
	}

	close(fd);

	return 0;
}
