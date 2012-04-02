#include <stdlib.h>	// atoi ()
#include <gtk/gtk.h>

#include "ThreadClass.h"


ThreadClass::ThreadClass() {
    if (!g_thread_supported ())
        g_thread_init (NULL);
    ThreadId_ = NULL;
}

ThreadClass::~ThreadClass()
{

}

bool ThreadClass::Start(void * arg)
{

//   int code = thread_create(ThreadClass::EntryPoint, this, &ThreadId_);
//   return code;
    Arg(arg); // store user data
    GError * err = NULL ;      
    if ( (ThreadId_ = g_thread_create ( ThreadClass::EntryPoint, this, TRUE, &err ) ) == NULL ){
        printf ( "Thread create failed: %s!!\n", err->message );
        g_error_free ( err ) ;
        return false;
    }
    return true;
}


/*static */
void * ThreadClass::EntryPoint(void * pthis)
{
	ThreadClass * pt = (ThreadClass*)pthis;
	pt->Execute( pt->Arg() );
	return NULL;
}

void ThreadClass::setPriority ( unsigned char pri )
{
    GThreadPriority priority = G_THREAD_PRIORITY_NORMAL;

    if ( pri == PRI_LOW )
        priority = G_THREAD_PRIORITY_LOW;
    else if ( pri == PRI_HIGH )
        priority = G_THREAD_PRIORITY_HIGH; 

    if ( ThreadId_ != NULL )
        g_thread_set_priority ( ThreadId_, priority ) ;
}

void ThreadClass::yield ()
{
    g_thread_yield () ;
}

void ThreadClass::join ()
{
    if ( ThreadId_ != NULL )
        g_thread_join ( ThreadId_ ) ;
}

void ThreadClass::Execute(void* arg)
{
        /// Your code goes here
}
