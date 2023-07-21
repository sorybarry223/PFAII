#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


 
int main(void)
{
	char buf[100]={0};
	int fd ;
	int num ;
	int mean=0;
	while(1)
	{
		fd= open("/proc/mydev", O_RDWR);
		lseek(fd, 0 , SEEK_SET);
		for (int k=0;k<100;k++)
		{
			read(fd, buf, 100);
			num= atoi(buf);
			mean+=num;
		
			
		}
		
		//printf("%s",buf);
		printf("%d	µs\n",mean/100000);
		mean=0;
		sleep(1);
		//puts(buf);
		//memset(buf, 0, sizeof buf);//buf[50]={0};
		close(fd);
		
	}
	return EXIT_SUCCESS;

}	

