#ifndef WRITE_THREAD_H_
#define WRITE_THREAD_H_

#include "ThreadClass.h"
#include "FifoBuffer.h"
#include "SelfImage_TImageStore.h"

// Some defines
#define WRITE_BUF_SIZE  2097152

#define ERROR_WRITETHREAD_NONE          20
#define ERROR_WRITETHREAD_FAIL          21
#define ERROR_WRITETHREAD_COMPRESS      22
#define ERROR_WRITETHREAD_DECOMPRESS    23


class TWriteThread : public ThreadClass{
	private:
		bool			AbortThread;
		TImageStore		*Target;
		TFifoBufferClass	*ImageBuffer;
		GThread			*ReadThread;
		TCopyData		*Data;
		int			LastError;
		TMutexClass		ErrorMutex;
		uint64_t		TotalBytesWritten;
		TMutexClass		BytesWrittenMutex;
		time_t			StartTime;


		bool WriteNocompression (void);
		bool WriteCompressGzip (void);
		void setLastError (int Error);
	protected:
		void Execute(void* arg);

	public:
		TWriteThread ();
		~TWriteThread ();
		int GetLastError (void);
		uint64_t GetNumbBytesWritten (void);
		const char *GetAverageSpeedString (void);
		const char *GetNumbBytesWrittenString (void);
		void Abort (void);
};

#endif // WRITE_THREAD_H_
