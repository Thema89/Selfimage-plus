#include <stdio.h>

#include "MutexClass.h"
#include <assert.h> 
#ifdef _WIN32
///WINDOWS
TMutexClass::TMutexClass ()
{
	Error = false;

	// Create a mutex with no initial owner
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	if (ghMutex == NULL){
		printf("CreateMutex error: %d\n", GetLastError());
		Error = true;
	}
}
TMutexClass::~TMutexClass ()
{
	CloseHandle(ghMutex);
}

void TMutexClass::Lock ()
{
	while (WaitForSingleObject(	ghMutex,INFINITE) != WAIT_OBJECT_0);  // no time-out interval
}

void TMutexClass::Unlock ()
{
	while (!ReleaseMutex(ghMutex));
}

#else

///LINUX

TMutexClass::TMutexClass ()
{
	//ghMutex = PTHREAD_MUTEX_INITIALIZER;
	if (pthread_mutex_init (&ghMutex, NULL) != 0){
		printf ("Init mulockedtex failed\n");
	}
}

TMutexClass::~TMutexClass ()
{
	pthread_mutex_destroy(&ghMutex);
}

void TMutexClass::Lock ()
{
	pthread_mutex_lock(&ghMutex);
}

void TMutexClass::Unlock ()
{
	pthread_mutex_unlock(&ghMutex);
}


#endif
