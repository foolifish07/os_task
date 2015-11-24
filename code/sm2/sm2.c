#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define MAX_THREAD_NUM  2

void control_thread( void );
void worker_thread( void );


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
			control_thread();
		else if ( ch == '2')
			return 0;
		printf("\nPress any key to finish this Program. \nThank you test this Proggram!\n");
		_getch();
	} //end while
} //end main

void control_thread( void )
{
DWORD n_thread = 2;
DWORD thread_ID;
DWORD wait_for_all;

// Tread Object Array

HANDLE h_Thread[MAX_THREAD_NUM];

// Create thread
	h_Thread[0] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(worker_thread), 0, 0, &thread_ID);
	h_Thread[1] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(worker_thread), 0, 0, &thread_ID);

// waiting all thread will been finished

wait_for_all = WaitForMultipleObjects(n_thread,h_Thread,TRUE, -1);
printf("All threads have finished Operating.\n");
}// end


// thread
void worker_thread(void)
{	
	int i;
	for( i=0; i<10; i++)
	{
		printf("x= %05d\n",i);
		Sleep(1000);  //wait a moment
	}
}