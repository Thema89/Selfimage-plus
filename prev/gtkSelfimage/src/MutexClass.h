#ifndef MUTEX_CLASS_H_
#define MUTEX_CLASS_H_

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <stdio.h>

class TMutexClass{
	private:
		#ifdef _WIN32
		HANDLE ghMutex;
		#else
		pthread_mutex_t ghMutex;
		#endif
		bool    Error;
	public:
		TMutexClass ();
		~TMutexClass ();
		void Lock (void);
		void Unlock (void);

};

#endif //MUTEX_CLASS_H_
