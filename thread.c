#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <stdint.h>
#define BUFSIZE  50
#define pwm10 "/sys/class/pwm/pwmchip1/pwm0/"
#define pwm1 "/sys/class/pwm/pwmchip1/"
#include <time.h>   /* for timer_create(), `struct itimerspec`,
                     * timer_t and CLOCK_REALTIME
                     */
                     
        char info[] = "100ms elapsed.";
        timer_t timerid;
        struct sigevent sev;
        struct itimerspec trigger;
        int fd;

long int val=0;
       	int df ;
	char file[100] ; 

pthread_cond_t cv0,cv1,cv2;
pthread_mutex_t lock;
void PI_reg(long int r) 
{
	char rs[50];
	strcpy(file, pwm10);
	strcat(file,"duty_cycle");
	//printf("%s",file);
	df= open(file, O_WRONLY);
	sprintf(rs,"%ld",r);
	write(df, rs,strlen(rs));//10µs
	close(df);
}

void thread_timer100(union sigval sv)
 {
        char *s = sv.sival_ptr;


        /* Will print "100ms elapsed." */
    //puts(s);
    pthread_mutex_lock(&lock);
pthread_cond_signal(&cv0);
pthread_mutex_unlock(&lock);

        timer_settime(timerid, 0, &trigger, NULL);
}

void *thread0(void *v) //GPIO
{
      //static unsigned int valeur=0;
for(;;)
{

pthread_mutex_lock(&lock);
pthread_cond_wait(&cv0, &lock);
pthread_mutex_unlock(&lock);
//printf("GPIO toggled\n");
//------toggle GPIO -------
 
//-------------------------

pthread_mutex_lock(&lock);
pthread_cond_signal(&cv1);
pthread_mutex_unlock(&lock);

}
return NULL;
}

void *thread1(void *v) //ADC
{

for(;;)
{

pthread_mutex_lock(&lock);
pthread_cond_wait(&cv1, &lock);
pthread_mutex_unlock(&lock);
//printf("ADC read ");
//------ read Velocity -------

char T[BUFSIZE];
fd = open("/proc/mydev", O_RDONLY);
if (fd == -1) {perror("Unable to open gpio->value");exit(1);}
read(fd, T, strlen(T)) ;
close (fd);
val= atoi(T);
printf("T = %s\n",T);
//-------------------------
pthread_mutex_lock(&lock);
pthread_cond_signal(&cv2);
pthread_mutex_unlock(&lock);

}
return NULL;
}
void *thread2(void *v)// PWM
 {
for(;;)
{

pthread_mutex_lock(&lock);
pthread_cond_wait(&cv2, &lock);
pthread_mutex_unlock(&lock);
//printf(" PWM updated\n");
//------update PWM duty -------
//Régulateur PI
PI_reg(val);
//-------------------------

}
return NULL;
}

int main (int argc, char** argv)
{

char cmd[1024];
pthread_t *t0,*t1,*t2;



t0 = (pthread_t *) malloc(sizeof(pthread_t));
t1 = (pthread_t *) malloc(sizeof(pthread_t));
t2 = (pthread_t *) malloc(sizeof(pthread_t));

//printf("Type lock to run a thread that locks and waits.\n");
printf("tape S to stop.\n");

pthread_create(t0, NULL, thread0, NULL);
pthread_create(t1, NULL, thread1, NULL);
pthread_create(t2, NULL, thread2, NULL);
/* Set all `sev` and `trigger` memory to 0 */
        memset(&sev, 0, sizeof(struct sigevent));
        memset(&trigger, 0, sizeof(struct itimerspec));

        /*
         * Set the notification method as SIGEV_THREAD:
         *
         * Upon timer expiration, `sigev_notify_function` (thread_handler()),
         * will be invoked as if it were the start function of a new thread.
         *
         */
        sev.sigev_notify = SIGEV_THREAD;
        sev.sigev_notify_function = &thread_timer100;
        sev.sigev_value.sival_ptr = &info;

        /* Create the timer. In this example, CLOCK_REALTIME is used as the
         * clock, meaning that we're using a system-wide real-time clock for
         * this timer.
         */
        timer_create(CLOCK_REALTIME, &sev, &timerid);

        /* Timer expiration will occur withing 5 seconds after being armed
         * by timer_settime().
         */
        trigger.it_value.tv_sec = 0;
        trigger.it_value.tv_nsec = 100000000;  // 100ms

        /* Arm the timer. No flags are set and no old_value will be retrieved.
         */
        timer_settime(timerid, 0, &trigger, NULL);
        
    strcpy (file , pwm1);
	strcat(file,"export");
	printf ("%s",file);
	df  = open (file,O_WRONLY);
	write (df, "0",1);
	close (df);    
 
	strcpy (file , pwm10);	
	strcat(file,"enable");
	printf ("%s",file);
	df  = open (file,O_WRONLY);
	write (df, "1",1);
	close (df);
	
	strcpy (file , pwm10);	
	strcat(file,"period");
	printf ("%s",file);
	df  = open (file,O_WRONLY);
	write (df, "2000000",7);//10µs
	close (df);
       


while(fscanf(stdin, "%s", cmd) != EOF) {
if((strcmp(cmd, "S") == 0)||(strcmp(cmd, "s") == 0))
{
       /* Delete (destroy) the timer */
        timer_delete(timerid);
exit(1);
}

}

}
