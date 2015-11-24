#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LEN  80


int main(int argc,char *argv[]) {
	
	/* Creat a new process to execute */
	char cmdLine[MAX_LINE_LEN];

	/* CreateProcess parameters */
	LPCTSTR lpApplicationName = NULL;
	LPSECURITY_ATTRIBUTES processSA = NULL;		/* Default */
	LPSECURITY_ATTRIBUTES threadSA = NULL;		/* Default */
	BOOL shareRights = TRUE;					/* Default */	
	DWORD creationMask =	CREATE_NEW_CONSOLE;	/* Window per process */
	LPVOID environment = NULL;					/* Default */
	LPTSTR curDir = NULL;						/* Default */
	
	STARTUPINFO startInfo;						/* Result */	
	PROCESS_INFORMATION processInfo;			/* Result */

	ZeroMemory(&startInfo,sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);

	FILE* fin = fopen("data/batch" , "r");
	/* 2.copy the command from command line parameters */
	while ( fscanf(fin , "%s" , cmdLine )!=EOF ){
		
		if(!CreateProcess(	
							lpApplicationName,	/* File name of executable */
							cmdLine,			/* Command line */
							processSA,			/* Process inherited security */
							threadSA,			/* Thread inherited security */
							shareRights,		/* Right propagation */
							creationMask,		/* Various creation flags */
							environment,		/* Environment variabkesr */
							curDir,				/* Child's current directory */
							&startInfo,
							&processInfo
						 )
		  ) {
				fprintf(stderr,"CreatProcess failed on error %d with %s\n",
						GetLastError() , cmdLine);
				ExitProcess(1);
			};
		fprintf(stdout,"The Child Process's PID: %d.\n", processInfo.dwProcessId);
		fprintf(stdout,"The Parent Process finish.\n");
		Sleep(500);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	};
	Sleep(5000);	

	return (0);
}