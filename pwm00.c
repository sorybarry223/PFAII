/*
 * pwm0.c
 * 
 * Copyright 2020 root <root@localhost.localdomain>
 *                                                                        15/15 

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define pwm0 "/sys/class/pwm/pwmchip0/"
#define pwm00 "/sys/class/pwm/pwmchip0/pwm0/"
int main(int argc, char **argv)
{
	int df ;
	char file[100] ;
	
	strcpy (file , pwm0);
	strcat(file,"export");
	printf ("%s",file);
	df  = open (file,O_WRONLY);
	//if(df == -1) printf("error opening file");
	write (df, "0",1);
	close (df);

	strcpy (file , pwm00);	
	strcat(file,"enable");
	printf ("%s",file);
	df  = open (file,O_WRONLY);
	write (df, "1",1);
	close (df);
	
	strcpy (file , pwm00);	
	strcat(file,"period");
	printf ("%s",file);
	df  = open (file,O_WRONLY);
	write (df, "300000",6);//10µs
	close (df);
	
	
	strcpy(file, pwm00);
	strcat(file,"duty_cycle");
	printf("%s",file);
	df= open(file, O_WRONLY);
	write(df, "200000",6);//10µs
	close(df);
	

	
	
}

