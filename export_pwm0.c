#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define pwm0 "/sys/class/pwm/pwmchip0/"
int main(int argc, char **argv)
{
int df ;
char file[1000] ;
strcpy (file , pwm0);
strcat(file,"export");
printf ("%s",file);
df  = open (file,O_WRONLY);
write (df, "0",1);
close (df);

}
