#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include "tlc5947.h"

void setDigit(int , int , int );

//debug showing off nixie. Tubes used has 0-9, left dot and right dot,so 12 digits
void showAll()
{
	int yy=0;
	while( yy < 12)
	{
		clearLeds();
		setDigit(0, yy, 800);
		usleep(1);
		setDigit(1, yy, 800);
		updateLeds();
		sleep(1);
		yy++;
	}
}

/*
get the current time
Assumming buff is 14
*/
int   get_time( int * buff)
{
	time_t now = time(&now);
	if( now <= 0 ){
		printf("no time returned");
		return (-1);
	}
	//printf("now = %ld\n", now);
	struct tm *newtm = localtime(&now);
	if ( newtm == 0){
		printf("bad struct from localtime\n");
		return -1;
	}
         newtm->tm_year = newtm->tm_year+1900;
         newtm->tm_mon = newtm->tm_mon+1;
	 buff[10] = newtm->tm_year/1000;
	 buff[11] = (newtm->tm_year%1000)/100;
	 buff[12] = (newtm->tm_year%100)/10;
	 buff[13] = newtm->tm_year%10;

	 buff[8] = newtm->tm_mon/10;
	 buff[9] = newtm->tm_mon%10;

	 buff[6] = newtm->tm_mday/10;
	 buff[7] = newtm->tm_mday%10;

	 buff[4] = newtm->tm_hour/10;
	 buff[5] = newtm->tm_hour%10;

	 buff[2] = newtm->tm_min/10;
	 buff[3] = newtm->tm_min%10;

	 buff[0] = newtm->tm_sec/10;
	 buff[1] = newtm->tm_sec%10;
	
	//printf("now %d %d\n", now, newtm->tm_sec);
	return 1;
}

/*
The board uses the tlc5947 which has 24 channels and we use 12 for each nixie.
So, (digitposition * 12) + value is the position.

Each of the 12 digits of the nixie is treated like a LED

If you change the number of tlc5947s, change  TLC_NUM in tlc5947.h
*/
void setDigit(int nixie, int val, int brightness)
{
	int which = (nixie*12) + val;
	setLed(which, brightness);
}

/* another clock */
int main(int argc, char * argv[])
{

	tlc5947init();
	 
	
	//showAll();
	
	//Buffer for all the digits we care about for time
	int tm_ar[14];
	/*
        Order of what is displayed:
	year
	month day
	hour min
	min sec
	
	*/
        //the digits and the 5th element are the number of seconds to show the data
	int what[4][5] = {{13,12,11,10,2}, {7,6, 9,8, 2}, {3,2, 5,4, 5}, {1,0, 3,2, 3}};

	//printf("arg %d\n", argc);
	while ( 1){ //forever
		//printf("\n------------------------\n");
		for( int xx=0 ; xx <=3; xx++) //each set of digits
		{
			clearLeds();
			usleep(10);
			if ( get_time(tm_ar) == -1){
		  		printf("Bad time");
				continue;
		 	 }	
			 //printf("arr [%d][%d]  %d  %d \n", 0, xx, what[xx][0], tm_ar[what[xx][0]]);
			 //printf("arr [%d][%d]  %d  %d \n", 1, xx, what[xx][1], tm_ar[what[xx][1]]);
			 //printf("arr [%d][%d]  %d  %d \n", 2, xx, what[xx][2], tm_ar[what[xx][2]]);
			 //printf("arr [%d][%d]  %d  %d \n \n", 3, xx, what[xx][3], tm_ar[what[xx][3]]);
			 setDigit(0, tm_ar[what[xx][0]], 800);
			 usleep(10);
			 setDigit(1, tm_ar[what[xx][1]], 800);
			 usleep(10);
			 setDigit(2, tm_ar[what[xx][2]], 800);
			 usleep(10);
			 setDigit(3, tm_ar[what[xx][3]], 800);
			 usleep(10);
			 updateLeds();

			 //printf("sleep %d\n", what[xx][4]);
			 sleep(what[xx][4]);
		}  
	}
	
	tlc5947cleanup();	
}


