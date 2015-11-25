#include <windows.h>
#include <conio.h>
#include <fstream>
#include <stdio.h>
#include <cmath>

#define INTE_PER_SEC  1000
#define MAX_THREAD_NUM  64
#define MAX_FILE_NUM  32
#define MAX_STR_LEN  32

using namespace std;
const int N = 9;

struct ThreadInfo
{
	int	serial;
	char entity;
	int a , b;
};

int prime[N] , in , out;

HANDLE mutex = CreateSemaphore( NULL,1,1,"mutex" );
HANDLE full = CreateSemaphore( NULL,0,N,"full" );
HANDLE empty = CreateSemaphore( NULL,N,N,"empty" );

int consumer_count = 0;
CRITICAL_SECTION  consumer;
int producer_count = 0;
CRITICAL_SECTION  producer;

void init(){
	in = out = 0;
	InitializeCriticalSection(&consumer);	
	InitializeCriticalSection(&producer);	
}
int P(HANDLE &mutex){
	WaitForSingleObject( mutex,-1);
}
int V(HANDLE &mutex){
	ReleaseSemaphore( mutex , 1, NULL );
}

void task4( char* file );
void DisplayThread(void* p);
void WriterThread(void* p);

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
			task4( (char*)"ex4.dat");
		else if ( ch == '2')
			return 0;
		printf("\nPress any key to finish this Program. \nThank you test this Proggram!\n");
		_getch();
	} //end while
} //end main


void task4( char* file )
{
	DWORD n_thread = 0;
	DWORD thread_ID;
	DWORD wait_for_all;

	// Tread Object Array

	HANDLE h_Thread[MAX_THREAD_NUM];
	ThreadInfo  thread_info[MAX_THREAD_NUM];

	ifstream  inFile;
	inFile.open(file);		//open file
	printf( "Task 4 :\n\n" );
	while ( inFile )
	{
		// read every reader/writer info
		inFile>>thread_info[n_thread].serial;
		inFile>>thread_info[n_thread].entity;
		inFile>>thread_info[n_thread].a;
		inFile>>thread_info[n_thread++].b;
		inFile.get();
	} //end while
	for( int i = 0; i < (int)(n_thread); i++)
	{
		if(thread_info[i].entity == 'D' || thread_info[1].entity == 'd')
		{
		// Create Display thread
			producer_count++;
	    	h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(DisplayThread), &thread_info[i], 0, &thread_ID);
		}	
		else {
		// Create Writer thread
			consumer_count++;
	   		h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(WriterThread), &thread_info[i], 0, &thread_ID);
		}
	} //end for

	// waiting all thread will been finished

	wait_for_all = WaitForMultipleObjects(n_thread,h_Thread,TRUE, -1);
	printf("All display and writer thread have finished Operating.\n");
}

void DisplayThread(void* p)
{
	int m_serial = ((ThreadInfo*) (p)) -> serial;
	int a = ((ThreadInfo*) (p)) -> a;
	int b = ((ThreadInfo*) (p)) -> b;

	for(int i=1;i<=a;i++){
		if ( consumer_count==0 ) break;
		P(full);
		P(mutex);

		printf("I'm comsumer %d , I get prime %d with out = %d.\n" , m_serial , prime[out] ,out );
		out = (out+1)%N;
		Sleep(a*100);
		
		V(mutex);
		V(empty);
	}

	EnterCriticalSection( &producer );
	producer_count--;
	LeaveCriticalSection( &producer );
}

bool isprime(int x){
	if ( x<=1 ) return false;
	int sq = sqrt(x);
	for(int i=2;i<=sq;i++){
		if ( x%i==0 ) return false;
	}
	return true;
}
void WriterThread(void* p)
{

	int m_serial = ((ThreadInfo*) (p)) -> serial;
	int a = ((ThreadInfo*) (p)) -> a;
	int b = ((ThreadInfo*) (p)) -> b;

	for(int i=a;i<=b;i++){
		if ( !isprime(i) ) continue;

		if ( producer_count==0 ) break;

		P(empty);
		P(mutex);

		prime[in] = i;
		in = (in+1)%N;
		printf("I'm producer %d , I get prime %d with in = %d.\n" , m_serial , i,in );
		Sleep(a*100);
		
		V(mutex);
		V(full);
	}

	EnterCriticalSection( &consumer );
	consumer--;
	LeaveCriticalSection( &consumer );
}
