#include <stdlib.h>	// atoi ()
#include <gtk/gtk.h>
#include <zlib.h>

#include "./alert/alert.h" 
#include "WriteThread.h"
#include "SelfImage_TypesDefs.h"
#include "SelfImage_Utility.h"


#define GZ_CHUNK_SIZE 16384

//**************************************************************************
// Constructor
TWriteThread::TWriteThread ()
{
    Target          = NULL;
    ImageBuffer     = NULL;
    LastError       = ERROR_WRITETHREAD_NONE;
    TotalBytesWritten    = 0;
    AbortThread   = false;
    StartTime       = time(0);
}

//**************************************************************************
// Desctructor
TWriteThread::~TWriteThread ()
{
    if (Target != NULL){
       delete Target;
       Target = NULL;
    }
}

//**************************************************************************
int TWriteThread::GetLastError (void)
{
int Error;

	// If we have not written to this memroy return it
	if (LastError == ERROR_WRITETHREAD_NONE)
		return ERROR_WRITETHREAD_NONE;

	ErrorMutex.Lock();
	Error = LastError;
//	LastError = ERROR_WRITETHREAD_NONE;
	ErrorMutex.Unlock();

	return Error;
}

//**************************************************************************
uint64_t TWriteThread::GetNumbBytesWritten (void)
{
uint64_t Bytes;

	BytesWrittenMutex.Lock();
	Bytes = TotalBytesWritten;
	BytesWrittenMutex.Unlock();
	return Bytes;
}

//**************************************************************************
const char *TWriteThread::GetAverageSpeedString (void)
{
std::string AvSpeedLabel;

	if ((time(0)-StartTime) > 0)
		AvSpeedLabel = MakeByteLabel(TotalBytesWritten/(time(0)-StartTime));
	AvSpeedLabel += "/s";
	return AvSpeedLabel.c_str();
}

//**************************************************************************
const char *TWriteThread::GetNumbBytesWrittenString (void)
{
std::string NumbBytesWrittenLabel;

	NumbBytesWrittenLabel = MakeByteLabel(TotalBytesWritten);
	return NumbBytesWrittenLabel.c_str();
}

//**************************************************************************
void TWriteThread::Abort (void)
{
    AbortThread   = true;
}

//**************************************************************************
void TWriteThread::setLastError (int Error)
{
	ErrorMutex.Lock();
	LastError   = Error;
	ErrorMutex.Unlock();
}

//**************************************************************************
/// TODO this function needs to be ported to using the write thread method of work
/// and the ImageBuffer!!!
bool TWriteThread::WriteCompressGzip (void)
{
unsigned long int BytesRead = 0, BytesWritten = 0;
int ret, flush;
unsigned have;
z_stream strm;
unsigned char in[GZ_CHUNK_SIZE];
unsigned char out[GZ_CHUNK_SIZE];
unsigned int    DataSize        = GZ_CHUNK_SIZE;
//static gz_header    gzHeader = {false, 0, 0, 255, NULL, 8, 0, NULL, 0, NULL, 0, false, false};

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	//#define Z_BEST_SPEED             1
	//#define Z_BEST_COMPRESSION       9
	//#define Z_DEFAULT_COMPRESSION  (-1)
	ret = deflateInit2(&strm, Data->CompressionMethod, Z_DEFLATED, 31, 9, Z_DEFAULT_STRATEGY);
	if (ret != Z_OK){
		setLastError (ERROR_WRITETHREAD_FAIL);
		Abort ();
		return false;
	}

/*
    ret = deflateSetHeader(&strm, (gz_headerp)&gzHeader);
    if (ret != Z_OK){
		setLastError (ERROR_WRITETHREAD_FAIL);
		Abort ();
		return false;
    }
*/
	/* compress until end of file */
	do {
		BytesRead = 0;
		if (ImageBuffer->Done()){
			BytesRead = 0;
		}else{

			// Get data from the buffer
			while (!ImageBuffer->GetData ((char*)in, DataSize) && !AbortThread){

				// Handle draining the Image buffer
				if (ImageBuffer->GetWriteFinished () && ImageBuffer->GetSizeOfDataInBuffer () <= DataSize){
					DataSize = ImageBuffer->GetSizeOfDataInBuffer ();
				}else
					yield();
			}

			// Handle AbortThreading the thread
			if (!AbortThread)
				BytesRead = DataSize;
		}

		strm.avail_in = BytesRead;

		//flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
		if (BytesRead == 0)
			flush = Z_FINISH;
		else
			flush = Z_NO_FLUSH;

		strm.next_in = in;

		/* run deflate() on input until output buffer not full, finish
		compression if all of source has been read in */
		do {
			strm.avail_out = GZ_CHUNK_SIZE;
			strm.next_out = out;
			ret = deflate(&strm, flush);    /* no bad return value */
			//assert(ret != Z_STREAM_ERROR);  /* state not clobbered */

			/* state not clobbered */
			if (ret == Z_STREAM_ERROR){
				setLastError (ERROR_WRITETHREAD_COMPRESS);
				Abort ();
				(void)deflateEnd(&strm);
				return false;
			}


			have = GZ_CHUNK_SIZE - strm.avail_out;

			if (!Target->Write((void*)out, have, &BytesWritten)){
				setLastError (ERROR_WRITETHREAD_COMPRESS);
				Abort ();
				(void)deflateEnd(&strm);
				alert ("Here");
				return false;
			}

			#ifndef _WIN32
			//sync();
			#endif

			if (BytesWritten != have){
				setLastError (ERROR_WRITETHREAD_COMPRESS);
				Abort ();
				(void)deflateEnd(&strm);
				return false;
			}
			
			BytesWrittenMutex.Lock();
			TotalBytesWritten += BytesWritten;
			BytesWrittenMutex.Unlock();

		} while (strm.avail_out == 0 && !AbortThread);

//		assert(strm.avail_in == 0);     /* all input will be used */
		if (strm.avail_in !=0 && !AbortThread){
			setLastError (ERROR_WRITETHREAD_COMPRESS);
			Abort ();
			(void)deflateEnd(&strm);
			return false;
		}

	/* done when last data in file processed */
	} while (flush != Z_FINISH && !AbortThread);
	
	//assert(ret == Z_STREAM_END);        /* stream will be complete */
	if (ret != Z_STREAM_END && !AbortThread){
		setLastError (ERROR_WRITETHREAD_COMPRESS);
		Abort ();
		(void)deflateEnd(&strm);
		return false;
	}

	/* clean up and return */
	(void)deflateEnd(&strm);

	return true;
}

//**************************************************************************
bool TWriteThread::WriteNocompression (void)
{
char            *WriteBuffer    = NULL;
unsigned long   BytesWritten    = 0;
unsigned int    DataSize        = WRITE_BUF_SIZE;

	WriteBuffer = new char[WRITE_BUF_SIZE+300];

	BytesWrittenMutex.Lock();
	BytesWritten = 0;
	BytesWrittenMutex.Unlock();

	while (!ImageBuffer->Done() && !AbortThread){
		if (ImageBuffer->GetWriteFinished () && ImageBuffer->GetSizeOfDataInBuffer () < DataSize){
			DataSize = ImageBuffer->GetSizeOfDataInBuffer ();
		}

		if (ImageBuffer->GetData (WriteBuffer, DataSize)){
			if (!Target->Write(WriteBuffer, DataSize, &BytesWritten)){
				setLastError (ERROR_WRITETHREAD_FAIL);
				Abort ();
				if (WriteBuffer != NULL)
					delete[] WriteBuffer;
				return false;
            }

			#ifndef _WIN32
			//sync();
			#endif

			// Check that we have succesfully written the data
			if (BytesWritten != DataSize){
				setLastError (ERROR_WRITETHREAD_FAIL);
				Abort ();
				if (WriteBuffer != NULL)
					delete[] WriteBuffer;
				return false;
			}
			BytesWrittenMutex.Lock();
			TotalBytesWritten += BytesWritten;
			BytesWrittenMutex.Unlock();
		}
	}

	if (WriteBuffer != NULL)
		delete[] WriteBuffer;

	return true;
}

//**************************************************************************
void TWriteThread::Execute(void* arg)
{

	Data = (TCopyData*) arg;
	AbortThread = false;
	setLastError (ERROR_WRITETHREAD_NONE);
	TotalBytesWritten = 0;
	ImageBuffer = Data->ImgBuffer;

	if ( ImageBuffer == NULL || Data == NULL ){
		/// SET SOME ERROR
		setLastError (ERROR_WRITETHREAD_FAIL);
		Abort ();
		return;
	}

	// Open the target file/drive
	if (Data->DestinationMediaType == isFile)
		Target = new TImageStore( Data->DestinationMediaType,  Data->Destination.c_str(), true );
	else
		Target = new TImageStore( Data->DestinationMediaType,  Data->Destination.c_str(), true );

	// Init the info var's
	BytesWrittenMutex.Lock();
	TotalBytesWritten = 0;
	BytesWrittenMutex.Unlock();

	StartTime = time(0);

	if (Data->CompressionMethod == Z_BEST_SPEED || Data->CompressionMethod == Z_BEST_COMPRESSION ){
		WriteCompressGzip ();
	}else{
		WriteNocompression ();
	}


	if (Target != NULL){
		delete Target;
		Target = NULL;
	}


	AbortThread = false;

}

