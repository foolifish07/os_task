#include <windows.h>
#include <conio.h>
#include <fstream.h>
#include <stdio.h>

#define MAX_THREAD_NUM  64


struct ThreadInfo {
	int	serial;
	int	start;
	int end;
} ;

void control_thread ( char* file);
void worker_thread  ( void* p );


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
			control_thread("sm3.dat");
		else if ( ch == '2')
			return 0;
		printf("\nPress any key to finish this Program. \nThank you test this Proggram!\n");
		_getch();
	} //end while
} //end main

void control_thread( char* file)
{
DWORD n_thread = 0;
DWORD thread_ID;
DWORD wait_for_all;

int i;

// Thread Object Array

HANDLE h_Thread[MAX_THREAD_NUM];
struct ThreadInfo  thread_info[MAX_THREAD_NUM];

ifstream  inFile;
inFile.open(file);		//open file
printf( "Now, We begin to Read thread Information to thread_info array \n\n" );

while ( inFile )
{
	// read every thread info
	inFile>>thread_info[n_thread].serial;
	inFile>>thread_info[n_thread].start;
	inFile>>thread_info[n_thread++].end;
	inFile.get();
} //end while

// Create all thread
for(  i = 0; i < (int)(n_thread); i++)
{
	// Create a thread
    	h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(worker_thread), &thread_info[i], 0, &thread_ID);
} //end for
// Create thread
	
// waiting all thread will been finished

wait_for_all = WaitForMultipleObjects(n_thread,h_Thread,TRUE, -1);
printf("All threads have finished Operating.\n");
}// end control_thread

// thread

void worker_thread ( void* p )
{
	int i;
	int m_serial;
	int m_start;
	int m_end;

//get info froam para

	m_serial = ((ThreadInfo*) (p)) -> serial;
	m_start  = int (((ThreadInfo*)(p)) -> start);
	m_end  = int (((ThreadInfo*)(p)) -> end);
	
	for( i=m_start; i <= m_end; i++)
	{
		printf("I am thread  %d , I am doing  %05dth step\n",m_serial,i);
		Sleep(1000);
	}
}