#include <stdio.h>
#include <string.h>

#include "FifoBuffer.h"
#include "MutexClass.h"


// Constructor
TFifoBufferClass::TFifoBufferClass ()
{
    Empty();
}

// Destructor
TFifoBufferClass::~TFifoBufferClass()
{

}

void TFifoBufferClass::Empty(void)
{
    StartPointer = 0;
    EndPointer   = 0;
    WriteFinished = false;
}

// Check if we are done reading form this buffer
bool TFifoBufferClass::Done ()
{
	if (GetSizeOfDataInBuffer () > 0){
		return false;
	}
	BufferMutex.Lock ();
	if (WriteFinished){
		BufferMutex.Unlock ();
		return true;
	}
	BufferMutex.Unlock ();
	return false;
}

void TFifoBufferClass::SetWriteFinished (void)
{
    WriteFinished = true;
}
bool TFifoBufferClass::GetWriteFinished (void)
{
    return WriteFinished;
}

// Returns the size of the data in the buffer
unsigned int TFifoBufferClass::GetSizeOfDataInBuffer (void)
{
    BufferMutex.Lock ();
    if (StartPointer == EndPointer){
    	//g_mutex_unlock (BufferMutex);
        BufferMutex.Unlock ();
        return 0;
    }else if (StartPointer > EndPointer){
    	//g_mutex_unlock (BufferMutex);
        BufferMutex.Unlock ();
        return (BUFFER_SIZE-StartPointer + EndPointer);
    }else{
    	//g_mutex_unlock (BufferMutex);
        BufferMutex.Unlock ();
        return (EndPointer - StartPointer);
    }

    // We will never reach this point
//   	g_mutex_unlock (BufferMutex);
        BufferMutex.Unlock ();
    return 0;
}

// Returns the space left to store data in
unsigned int TFifoBufferClass::GetFreeSpaceInBuffer (void)
{
//   	g_mutex_lock (BufferMutex);
        BufferMutex.Lock ();
    if (StartPointer == EndPointer){
    	//g_mutex_unlock (BufferMutex);
        BufferMutex.Unlock ();
        return BUFFER_SIZE;
    }else if (StartPointer > EndPointer){
    	//g_mutex_unlock (BufferMutex);
        BufferMutex.Unlock ();
        return (StartPointer - EndPointer);
    }else{
    	//g_mutex_unlock (BufferMutex);
        BufferMutex.Unlock ();
        return (StartPointer + (BUFFER_SIZE - EndPointer));
    }

    // We will never reach this point
   	//g_mutex_unlock (BufferMutex);
        BufferMutex.Unlock ();
    return 0;
}

// Add data to the fifo buffer
bool TFifoBufferClass::AddData (char *Data, unsigned int size)
{
unsigned int TempSize;

//   	g_mutex_lock (BufferMutex);


    if (GetFreeSpaceInBuffer() <= size){
        //printf ("Not enough free space to add %i bytes, space = %u, used = %u\n", size, GetFreeSpaceInBuffer(), GetSizeOfDataInBuffer ());
    	//g_mutex_unlock (BufferMutex);
        return false;
    }

    BufferMutex.Lock ();

    while (size > 0){

        if ((EndPointer + size) > BUFFER_SIZE){
            TempSize = BUFFER_SIZE - EndPointer;
        }else{
            TempSize = size;
        }

        if (TempSize > size)
            printf ("%s line %i: ERROR, TempSize to big\n", __FILE__, __LINE__);

        memcpy (buffer+EndPointer, Data, TempSize);

        EndPointer += TempSize;
        Data += TempSize;

        if ( EndPointer >= BUFFER_SIZE )
            EndPointer = 0;

        size -= TempSize;
    }

//   	g_mutex_unlock (BufferMutex);
        BufferMutex.Unlock ();
    return true;
}

// Get data from the fifo buffer and remove the data from the buffer
bool TFifoBufferClass::GetData (char *Data, unsigned int size)
{
unsigned int SizeOfData;
unsigned int TempSize;

//  	g_mutex_lock (BufferMutex);

    SizeOfData = GetSizeOfDataInBuffer ();

        BufferMutex.Lock ();

    if (SizeOfData < size){
    	//g_mutex_unlock (BufferMutex);
        BufferMutex.Unlock ();
        return false;
    }

    while (size > 0){
        if (StartPointer + size > BUFFER_SIZE){
            TempSize = BUFFER_SIZE - StartPointer;
        }else{
            TempSize = size;
        }

        memcpy (Data, buffer+StartPointer, TempSize);

        StartPointer += TempSize;
        Data += TempSize;

        if (StartPointer >= BUFFER_SIZE)
            StartPointer = 0;

        size -= TempSize;
    }

//   	g_mutex_unlock (BufferMutex);
        BufferMutex.Unlock ();

	return true;
}


