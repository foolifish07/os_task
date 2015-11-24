#include <windows.h>
#include <conio.h>
#include <fstream>
#include <stdio.h>

#define INTE_PER_SEC  1000
#define MAX_THREAD_NUM  64
#define MAX_FILE_NUM  32
#define MAX_STR_LEN  32
using namespace std;

struct ThreadInfo
{
	int	serial;
	char	entity;
	double	delay;
	double	persist;
};

void ReaderPriority( char* file );
void RP_ReaderThread(void* p);
void RP_WriterThread(void* p);

CRITICAL_SECTION  critical_readcount;
int readcount = 0;
void init(){
	readcount = 0;
	InitializeCriticalSection(&critical_readcount);		
}

HANDLE mutex = CreateSemaphore( NULL,1,1,"mutex" );
void P(HANDLE &mutex){
	WaitForSingleObject( mutex,-1);
}
void V(HANDLE &mutex){
	ReleaseSemaphore( mutex , 1, NULL );
}


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
			ReaderPriority((char*)"ex5.dat");
		else if ( ch == '2')
			return 0;
		printf("\nPress any key to finish this Program. \nThank you test this Proggram!\n");
		_getch();
	} //end while
} //end main

///////////////////////////////////////////////////////////////
// Reader Priority fuction
// file: filename
//////////////////////////////////////////////////////////////

void ReaderPriority( char* file )
{
	init();

	DWORD n_thread = 0;
	DWORD thread_ID;
	DWORD wait_for_all;

	// Tread Object Array
	HANDLE h_Thread[MAX_THREAD_NUM];
	ThreadInfo  thread_info[MAX_THREAD_NUM];

	ifstream  inFile;
	inFile.open(file);		//open file
	printf( "Reader Priority:\n\n" );
	while ( inFile )
	{
		// read every reader/writer info
		inFile>>thread_info[n_thread].serial;
		inFile>>thread_info[n_thread].entity;
		inFile>>thread_info[n_thread].delay;
		inFile>>thread_info[n_thread++].persist;
		inFile.get();
	} //end while
	for( int i = 0; i < (int)(n_thread); i++)
	{
		if(thread_info[i].entity == 'R' || thread_info[1].entity == 'r')
		{
		// Create Reader thread
	    	h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(RP_ReaderThread), &thread_info[i], 0, &thread_ID);
		}	
		else {
		// Create Writer thread
	   		h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(RP_WriterThread), &thread_info[i], 0, &thread_ID);
		}
		printf("%d\n" , i);
	} //end for

	// waiting all thread will been finished

	wait_for_all = WaitForMultipleObjects(n_thread,h_Thread,TRUE, -1);
	printf("All reader and writer thread have finished Operating.\n");
}// end readerPriority

////////////////////////////////////
// reader priority -- reader thread
// p: reader thread info
///////////////////////////////////

void RP_ReaderThread(void* p)
{
	DWORD wait_for_mutex;
	DWORD m_delay;
	DWORD m_persist;
	int m_serial;

	//get info froam para

	m_serial = ((ThreadInfo*) (p)) -> serial;
	m_delay  = (DWORD) (((ThreadInfo*)(p)) -> delay*INTE_PER_SEC);
	m_persist = (DWORD) (((ThreadInfo*)(p)) -> persist*INTE_PER_SEC);

	Sleep(m_delay);
	printf("Reader thread %d sents the reading require .\n",m_serial);

	EnterCriticalSection( &critical_readcount );
	readcount++;
	if (readcount == 1 ){
		P(mutex);
	}
	LeaveCriticalSection( &critical_readcount );

	printf("Reader thread %d begins to read file.\n",m_serial);
	Sleep(m_persist);
	printf("Reader thread %d finished reading file.\n",m_serial);

	
	EnterCriticalSection( &critical_readcount );
	readcount--;
	if ( readcount == 0 )
	{
		V(mutex);
	}
	LeaveCriticalSection( &critical_readcount );
}

////////////////////////////////////
// Reader priority -- writer thread
// p: writer thread info
///////////////////////////////////

void RP_WriterThread(void* p)
{
	DWORD m_delay;
	DWORD m_persist;
	int m_serial;

	//get info froam para

	m_serial = ((ThreadInfo*) (p)) -> serial;
	m_delay  = (DWORD) (((ThreadInfo*)(p)) -> delay*INTE_PER_SEC);
	m_persist = (DWORD) (((ThreadInfo*)(p)) -> persist*INTE_PER_SEC);

	Sleep(m_delay);
	printf("Writer thread %d sents the writing require .\n",m_serial);

	P(mutex);

	printf("Writer thread %d begins to write to the file.\n",m_serial);
	Sleep(m_persist);


	printf("Writer thread %d finished Writing to the file.\n",m_serial);

	V(mutex);
}
