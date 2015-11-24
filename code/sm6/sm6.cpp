#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <fstream.h>
#include <stdio.h>

#define INTE_PER_SEC  1000
#define MAX_THREAD_NUM  64

struct ThreadInfo
{
	int	serial;
	double	delay;
};

volatile  int accnt1 = 0; /*  in the memory */
volatile  int accnt2 = 0;


void account( char* file);
void acc(void* p);

////////////////////////////////////////////////////////
// main fuction
////////////////////////////////////////////////////////

int main( int agrc, char* argv[] )
{
	char ch;

	while ( TRUE )
	{
		// Cleare screen
		system( "cls" );

		// display prompt info
		printf("*********************************************\n");
		printf("       1.Start test\n");
		printf("       2.Exit to Windows\n");
		printf("*********************************************\n");
		printf("Input your choice(1or2): ");
		
		// if the number inputed is error, retry!
		do{
			ch = (char)_getch(); 
		}while( ch != '1' && ch != '2');

		system ( "cls" );
		if ( ch == '1')
			account("sm6.dat");
		else if ( ch == '2')
			return 0;
		printf("\nPress any key to finish this Program. \nThank you test this Proggram!\n");
		_getch();
	} //end while
} //end main

void account( char* file)
{
DWORD n_thread = 0;
DWORD thread_ID;
DWORD wait_for_all;

// Tread Object Array

HANDLE h_Thread[MAX_THREAD_NUM];
ThreadInfo  thread_info[MAX_THREAD_NUM];

ifstream  inFile;
inFile.open(file);		//open file
printf( "Now, We begin to read thread Information to thread_info array \n\n" );

while ( inFile )
{
	// read every thread info
	inFile>>thread_info[n_thread].serial;
	inFile>>thread_info[n_thread++].delay;
	inFile.get();
} //end while

// Create all thread
for( int i = 0; i < (int)(n_thread); i++)
{
	// Create a thread
    	h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(acc), &thread_info[i], 0, &thread_ID);
} //end for
// Create thread
	
// waiting all thread will been finished

wait_for_all = WaitForMultipleObjects(n_thread,h_Thread,TRUE, -1);
printf("All threads have finished Operating.\n");
}// end account

void acc(void* p)
{
	DWORD m_delay;
	int m_serial;

	int rand_num, accnt,counter = 0;;


//get info froam para

	m_serial = ((ThreadInfo*) (p)) -> serial;
	m_delay  = (DWORD) (((ThreadInfo*)(p)) -> delay*INTE_PER_SEC);

	srand( (unsigned)((ThreadInfo*)(p)) -> delay );

	do {
		printf("I am thread  %d , I am doing  %05dth step\n",m_serial,counter);
		rand_num = rand();
		/* printf("rand_num =  %05d \n",rand_num); */
		//Sleep(m_delay);

//begin critical_section
		accnt1 = accnt1 - rand_num;
		Sleep(m_delay);
		accnt2 = accnt2 + rand_num;
		accnt = accnt1 + accnt2;
//critical_section  end

		counter++;
	} while ( (accnt == 0) && (counter<10)); 
	printf("Now accnt1+accnt2 =  %05d\n",accnt);
} //end acc
