#ifndef READTHREAD_H_
#define READTHREAD_H_

#include "ThreadClass.h"
#include "FifoBuffer.h"
#include "SelfImage_TImageStore.h"

// Some defines
#define READ_BUF_SIZE  2097152

#define ERROR_READTHREAD_NONE                   10
#define ERROR_READTHREAD_FAIL                   11
#define ERROR_READTHREAD_COMPRESS               12
#define ERROR_READTHREAD_DECOMPRESS             13
#define ERROR_READTHREAD_UNSUPPORTED_FORMAT     14


class TReadThread : public ThreadClass{
    private:
        bool                    AbortThread;
        TImageStore             *Source;
        TFifoBufferClass        *ImageBuffer;
        GThread                 *ReadThread;
        TCopyData               *Data;
        int                     LastError;
        TMutexClass             ErrorMutex;
        uint64_t                TotalBytesRead;
        TMutexClass             BytesReadMutex;
        __uint64                TotalSizeInBytes;

        bool ReadNocompression (void);
        bool ReadDecompressGzip (void);
	void setLastError (int Error);
    protected:
        void Execute(void* arg);

    public:
        TReadThread ();
        ~TReadThread ();
        int GetLastError (void);
        uint64_t GetNumbBytesRead (void);
        uint64_t GetTotalSizeInBytes (void);
        const char *GetNumbBytesReadString (void);
        float GetProgress (void);
        void Abort (void);
};



#endif // READTHREAD_H_
