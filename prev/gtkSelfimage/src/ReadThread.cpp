#include <stdlib.h>	// atoi ()
#include <gtk/gtk.h>
#include <zlib.h>
#ifndef _WIN32
#include <assert.h>
#endif


#include "ReadThread.h"
#include "SelfImage_TypesDefs.h"
#include "SelfImage_Utility.h"
#include "./alert/alert.h"

#define CHUNK_SIZE 16384


//**************************************************************************
TReadThread::TReadThread ()
{
	Source      = NULL;
	ImageBuffer = NULL;
	LastError   = ERROR_READTHREAD_NONE;
	TotalSizeInBytes = 0;
}

//**************************************************************************
TReadThread::~TReadThread ()
{
	if (Source != NULL){
		delete Source;
		Source = NULL;
	}
}

//**************************************************************************
int TReadThread::GetLastError (void)
{
int Error;

	if (LastError == ERROR_READTHREAD_NONE)
		return ERROR_READTHREAD_NONE;

	ErrorMutex.Lock();
	Error = LastError;
//	LastError = ERROR_READTHREAD_NONE;
	ErrorMutex.Unlock();

	return Error;
}

//**************************************************************************
uint64_t TReadThread::GetNumbBytesRead (void)
{
uint64_t Bytes;

	BytesReadMutex.Lock();
	Bytes = TotalBytesRead;
	BytesReadMutex.Unlock();
	return Bytes;
}

//**************************************************************************
uint64_t TReadThread::GetTotalSizeInBytes (void)
{
	return TotalSizeInBytes;
}

//**************************************************************************
void TReadThread::Abort (void)
{
	AbortThread   = true;
}

//**************************************************************************
float TReadThread::GetProgress (void)
{
float Progress;

	if (TotalBytesRead == 0 || TotalSizeInBytes == 0)
		return 0;

	BytesReadMutex.Lock();
	Progress = ((long double)TotalBytesRead/(long double)TotalSizeInBytes);
	BytesReadMutex.Unlock();

	return Progress;
}

//**************************************************************************
const char *TReadThread::GetNumbBytesReadString (void)
{
std::string NumbBytesReadLabel;

	NumbBytesReadLabel = MakeByteLabel(TotalBytesRead);
	return NumbBytesReadLabel.c_str();
}

//**************************************************************************
void TReadThread::setLastError (int Error)
{
	ErrorMutex.Lock();
	LastError   = Error;
	ErrorMutex.Unlock();
}

//**************************************************************************
/* Decompress from file source to file dest until stream ends or EOF.
   inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_DATA_ERROR if the deflate data is
   invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
   the version of the library linked do not match, or Z_ERRNO if there
   is an error reading or writing the files. */
bool TReadThread::ReadDecompressGzip (void)
{
unsigned long       BytesRead = 1;
int                 ret;
unsigned            have;
z_stream            zsStream;
int                 Retry;
unsigned char       in[CHUNK_SIZE+4];
unsigned char       out[CHUNK_SIZE+4];


	/* allocate inflate state */
	memset(&zsStream, 0, sizeof(z_stream));
	zsStream.zalloc = Z_NULL;
	zsStream.zfree = Z_NULL;
	zsStream.opaque = Z_NULL;
	zsStream.avail_in = 0;
	zsStream.next_in = Z_NULL;
	//static gz_header head;

    // Init inflate
	ret = inflateInit2(&zsStream, 31);
	if (ret != Z_OK){
		setLastError (ERROR_READTHREAD_DECOMPRESS);
		AbortThread = true;
		return false;
	}

/*
    // Get the header
    ret = inflateGetHeader (&zsStream, &head);
	if (ret != Z_OK){
		setLastError (ERROR_READTHREAD_DECOMPRESS);
		AbortThread = true;
		return false;
	}
*/

	/* decompress until deflate stream ends or end of file */
	do {
		if (!Source->Read((void*)in, CHUNK_SIZE, &BytesRead)){
		  setLastError (ERROR_READTHREAD_DECOMPRESS);
		  AbortThread = true;
		  return false;
        }

		zsStream.avail_in = BytesRead;

		BytesReadMutex.Lock();
		TotalBytesRead   += BytesRead;
		BytesReadMutex.Unlock();

		if (zsStream.avail_in == 0)
			break;
		zsStream.next_in = in;

		/* run inflate() on input until output buffer not full */
		do {
			zsStream.avail_out = CHUNK_SIZE;
			zsStream.next_out = out;
			ret = inflate(&zsStream, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR;     /* and fall through */
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					(void)inflateEnd(&zsStream);
					AbortThread = true;
					setLastError (ERROR_READTHREAD_DECOMPRESS);
					return false;
			}
			have = CHUNK_SIZE - zsStream.avail_out;
			Retry = 0;

			while (!ImageBuffer->AddData((char*)out, have) && !AbortThread){
				yield ();
			}

		} while (zsStream.avail_out == 0 && !AbortThread);

	/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END && !AbortThread);

	/* clean up and return */
	(void)inflateEnd(&zsStream);
	ret = ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;

	if (ret == Z_OK)
		return true;

	setLastError (ERROR_READTHREAD_DECOMPRESS);

	return false;
}

//**************************************************************************
bool TReadThread::ReadNocompression (void)
{
char                *ReadBuffer = NULL;
unsigned long       BytesRead = 1;
unsigned long       ReadBufSize = 0;
unsigned char       FailCounter = 0;

	ReadBufSize	= READ_BUF_SIZE;

	ReadBuffer = new char[READ_BUF_SIZE+300];

//	while ((BytesRead > 0 || FailCounter > 0) && !AbortThread){
	while (!Source->eof() && !AbortThread){
		if (!Source->Read((void*)ReadBuffer, ReadBufSize, &BytesRead)){
			if ((Source->GetTotalSizeInBytes () - TotalBytesRead) > ReadBufSize){
				// There is still more than enough data to read --> error
				// printf ("Size left = %llu, buf size = %lu\n", (Source->GetTotalSizeInBytes () - TotalBytesRead), ReadBufSize);
				setLastError (ERROR_READTHREAD_FAIL);
				AbortThread = true;
			}else{
				//if (Source->getType () == isDrive){
				//	ReadBufSize	= Source->GetBytesPerSector ();
				//}else{
				//	ReadBufSize	= 512;
				//}
				ReadBufSize = Source->GetTotalSizeInBytes () - TotalBytesRead;
				//printf ("Size left = %llu, buf size = %lu\n", (Source->GetTotalSizeInBytes () - TotalBytesRead), ReadBufSize);
			}
			FailCounter++;
			if (FailCounter > 2){
				setLastError (ERROR_READTHREAD_FAIL);
				AbortThread = true;
				return false;
			}
			continue;
		}else{
			FailCounter = 0;
		}

		while (!ImageBuffer->AddData(ReadBuffer, BytesRead) && !AbortThread){
			yield();
		}

		BytesReadMutex.Lock();
		TotalBytesRead += BytesRead;
		BytesReadMutex.Unlock();
	}

	if (ReadBuffer != NULL)
		delete[] ReadBuffer;

	return true;
}

//**************************************************************************
void TReadThread::Execute(void* arg)
{
	Data = (TCopyData*) arg;
	TotalBytesRead  = 0;
	setLastError (ERROR_READTHREAD_NONE);

	ImageBuffer = Data->ImgBuffer;

	if (ImageBuffer == NULL || Data == NULL ){
		/// SET SOME ERROR
		return;
	}

	ImageBuffer->Empty();

	// Open the source file/drive
	Source = new TImageStore( Data->SourceMediaType, Data->SourceDir.c_str() );

	if (Source == NULL)
		return;

	TotalSizeInBytes = Source->GetTotalSizeInBytes ();

	switch (Source->getCompressionFormat()){
		case cfNone:
			ReadNocompression ();
			break;
		case cfGzipStream:
			if ( Data->InflateSource )
				ReadDecompressGzip ();
			else
				ReadNocompression ();
			break;
		case cfGzipChunk:
			// Thes zip consists of many zip files, with our software only the first in extracted
			setLastError (ERROR_READTHREAD_UNSUPPORTED_FORMAT);
			break;
		default:
			break;
	}

	// Notify the write thread writing to the ImageBuffer has finished
	ImageBuffer->SetWriteFinished ();
/*
	if (Source->eof())
		printf ("EOF\n");
	else
		printf ("Not at end of file jet\n");
*/

	if (Source != NULL){
		delete Source;
		Source = NULL;
	}

	AbortThread = false;

}
