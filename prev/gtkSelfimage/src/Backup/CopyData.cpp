#include <stdlib.h>	// atoi ()
#include <gtk/gtk.h>
#include <string.h>
#include <fstream>
#include <time.h>

#include "CopyData.h"
#include "./alert/alert.h"
#include "./ui/myevents.h"
#include "DriveList.h"
#include "SelfImage_TImageStore.h"
#include "SelfImage_Utility.h"
#include "FifoBuffer.h"

// zlib includes
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zlib.h"

#define USE_ALERT
#define USE_FIFO_BUFFER

#ifdef USE_FIFO_BUFFER
TFifoBufferClass    FifoBuffer;
#endif

// Local var def's
TCopyData		            data;
GThread			            *CopyThread, *WriteThread;
volatile TProgressInfo      Info;
bool                        CopyThreadDone;
bool                        CopyThreadAbort;
bool                        DeflateInit = true;

// External var def's
extern GtkWidget *InputFileEntry;
extern GtkWidget *OutputFileEntry;
extern GtkWidget *InputFileRadioButton;
extern GtkWidget *InputDriveRadioButton;
extern GtkWidget *InputFileSelectButton;
extern GtkWidget *OutputFileRadioButton;
extern GtkWidget *OutputDriveRadioButton;
extern GtkWidget *OutputFileSelectButton;
extern GtkWidget *InputCobobox;
extern GtkWidget *OutputCobobox;
extern GtkWidget *CancelButton;
extern GtkWidget *pbar;
extern GtkWidget *InputFrame;
extern GtkWidget *ProcessingFrame;
extern GtkWidget *OutputFrame;
extern GtkWidget *StatusFrame;
extern GtkWidget *StatusInputSizeLabel;
extern GMutex	*ProcessInfoMutex;
extern GtkWidget *StatusInputPathLabel;
extern GtkWidget *StatusOutputPathLabel;
extern GtkWidget *StatusBytesWrittenLabel;
extern GtkWidget *StatusBytesReadLabel;
extern GtkWidget *StatusAverageSpeedLabel;
extern GtkWidget *CompressImageRadioButton;
extern GtkWidget *DeCompressSourceRadioButton;
extern GtkWidget *CompressionNoneRadioButton;
extern GtkWidget *GzipFastRadioButton;
extern GtkWidget *GzipBestRadioButton;

extern TDriveList	*DriveList;



//#define CHUNK_SIZE 16384
#define CHUNK_SIZE 16384



/* Decompress from file source to file dest until stream ends or EOF.
   inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_DATA_ERROR if the deflate data is
   invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
   the version of the library linked do not match, or Z_ERRNO if there
   is an error reading or writing the files. */
bool Copy_compressTargetDataGzip (TImageStore *Source, TImageStore *Target, unsigned char CompressionMethod)
{
unsigned long int BytesRead = 1, BytesWritten = 0;
int ret, flush;
unsigned have;
z_stream strm;
unsigned char in[CHUNK_SIZE];
unsigned char out[CHUNK_SIZE];

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	//#define Z_BEST_SPEED             1
	//#define Z_BEST_COMPRESSION       9
	//#define Z_DEFAULT_COMPRESSION  (-1)
	ret = deflateInit2(&strm, CompressionMethod, Z_DEFLATED, 31, 9, Z_DEFAULT_STRATEGY);
	if (ret != Z_OK)
		return false;

	/* compress until end of file */
	do {

		Source->Read((void*)in, CHUNK_SIZE, &BytesRead);

		strm.avail_in = BytesRead;


		g_mutex_lock (ProcessInfoMutex);
		Info.BytesRead += BytesRead;
		g_mutex_unlock (ProcessInfoMutex);


		//flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
		if (BytesRead == 0)
			flush = Z_FINISH;
		else
			flush = Z_NO_FLUSH;

		strm.next_in = in;

		/* run deflate() on input until output buffer not full, finish
		compression if all of source has been read in */
		do {
			strm.avail_out = CHUNK_SIZE;
			strm.next_out = out;
			ret = deflate(&strm, flush);    /* no bad return value */
			//assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            /* state not clobbered */
            if (ret == Z_STREAM_ERROR){
                g_mutex_lock (ProcessInfoMutex);
                Info.WriteThreadError = ERROR_WRITETHREAD_COMPRESS;
                g_mutex_unlock (ProcessInfoMutex);
                CopyThreadAbort = true;
				(void)deflateEnd(&strm);
				return false;
            }


			have = CHUNK_SIZE - strm.avail_out;

			Target->Write((void*)out, have, &BytesWritten);

			if (BytesWritten != have){
				//printf ("ERROR, Byteswritten = %i \n", BytesWritten);
                g_mutex_lock (ProcessInfoMutex);
                Info.WriteThreadError = ERROR_WRITETHREAD_COMPRESS;
                g_mutex_unlock (ProcessInfoMutex);
                CopyThreadAbort = true;
				(void)deflateEnd(&strm);
				return false;
			}
			g_mutex_lock (ProcessInfoMutex);
			Info.BytesWritten += BytesWritten;
			g_mutex_unlock (ProcessInfoMutex);

		} while (strm.avail_out == 0 && !CopyThreadAbort);
		assert(strm.avail_in == 0);     /* all input will be used */

	/* done when last data in file processed */
	} while (flush != Z_FINISH && !CopyThreadAbort);
	assert(ret == Z_STREAM_END);        /* stream will be complete */

	/* clean up and return */
	(void)deflateEnd(&strm);

	return true;
}


//
// The thread that writes the data
// WARNING: there should not be any gtk ui code in here !!!!
//
#ifdef USE_FIFO_BUFFER
#define WRITE_BUF_SIZE  2097152
void *WriteThreadProcess(void *args)
{
char            *WriteBuffer = NULL;
TImageStore     *Target;
unsigned long   BytesWritten = 0;
unsigned int    DataSize = WRITE_BUF_SIZE;

	// Get the arguments
	Target = (TImageStore *)args;

   	g_mutex_lock (ProcessInfoMutex);
	WriteBuffer = new char[WRITE_BUF_SIZE+300];
   	g_mutex_unlock (ProcessInfoMutex);

    Info.BytesWritten = 0;

    while (!FifoBuffer.Done() && !CopyThreadAbort){
        if (FifoBuffer.GetWriteFinished () && FifoBuffer.GetSizeOfDataInBuffer () < DataSize){
            DataSize = FifoBuffer.GetSizeOfDataInBuffer ();
        }

        if (FifoBuffer.GetData (WriteBuffer, DataSize)){
            Target->Write(WriteBuffer, DataSize, &BytesWritten);

            // Check that we have succesfully written the data
            if (BytesWritten != DataSize){
                    g_mutex_lock (ProcessInfoMutex);
                    Info.WriteThreadError = ERROR_WRITETHREAD_FAIL;
                    g_mutex_unlock (ProcessInfoMutex);
                    CopyThreadAbort = true;
                    break;
            }
            g_mutex_lock (ProcessInfoMutex);
            Info.BytesWritten += BytesWritten;
            g_mutex_unlock (ProcessInfoMutex);
        }
    }

    //printf ("Exit thread, abort = %i, size left = %i\n",CopyThreadAbort, FifoBuffer.GetSizeOfDataInBuffer ()); 

	if (WriteBuffer != NULL)
		delete[] WriteBuffer;

}
#endif


/* Decompress from file source to file dest until stream ends or EOF.
   inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_DATA_ERROR if the deflate data is
   invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
   the version of the library linked do not match, or Z_ERRNO if there
   is an error reading or writing the files. */
bool Copy_decompressSourceDataGzip (TImageStore *Source, TImageStore *Target)
{
unsigned long int BytesRead = 1, BytesWritten = 0;
	int ret;
	unsigned have;
	z_stream zsStream;
	int Retry;
	unsigned char in[CHUNK_SIZE+4];
	unsigned char out[CHUNK_SIZE+4];

#ifdef USE_FIFO_BUFFER
//char   WriteBuffer[CHUNK_SIZE+4];
GError              *error = NULL;
#endif

    // Create the write thread
	if( (WriteThread = g_thread_create((GThreadFunc)WriteThreadProcess, (void *)Target, TRUE, &error)) == NULL)
	{
		g_error_free ( error ) ;
        g_mutex_lock (ProcessInfoMutex);
        Info.WriteThreadError = ERROR_WRITETHREAD_FAIL;
        g_mutex_unlock (ProcessInfoMutex);
        CopyThreadAbort = true;
        return false;
	}

	/* allocate inflate state */
	memset(&zsStream, 0, sizeof(z_stream));
	zsStream.zalloc = Z_NULL;
	zsStream.zfree = Z_NULL;
	zsStream.opaque = Z_NULL;
	zsStream.avail_in = 0;
	zsStream.next_in = Z_NULL;
	//ret = inflateInit(&zsStream);
	ret = inflateInit2(&zsStream, 31);

	if (ret != Z_OK){
        g_mutex_lock (ProcessInfoMutex);
        Info.ReadThreadError = ERROR_READTHREAD_DECOMPRESS;
        g_mutex_unlock (ProcessInfoMutex);
        CopyThreadAbort = true;
		return false;
    }

	/* decompress until deflate stream ends or end of file */
	do {
		Source->Read((void*)in, CHUNK_SIZE, &BytesRead);
		zsStream.avail_in = BytesRead;

		g_mutex_lock (ProcessInfoMutex);
		Info.BytesRead += BytesRead;
		g_mutex_unlock (ProcessInfoMutex);


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
                    g_mutex_lock (ProcessInfoMutex);
                    Info.ReadThreadError = ERROR_READTHREAD_DECOMPRESS;
                    g_mutex_unlock (ProcessInfoMutex);
                    CopyThreadAbort = true;
					return false;
			}
			have = CHUNK_SIZE - zsStream.avail_out;
			//BytesWritten = WriteData (Target, out, have );
			BytesWritten = 0;
			Retry = 0;

#ifdef USE_FIFO_BUFFER
            while (!FifoBuffer.AddData((char*)out, have) && !CopyThreadAbort){
                g_thread_yield();
            }
#else
            Target->Write((void*)out, have, &BytesWritten);
   			g_mutex_lock (ProcessInfoMutex);
            Info.BytesWritten += BytesWritten;
            g_mutex_unlock (ProcessInfoMutex);

#endif

		} while (zsStream.avail_out == 0 && !CopyThreadAbort);

	/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END && !CopyThreadAbort);

    FifoBuffer.SetWriteFinished ();

	/* clean up and return */
	(void)inflateEnd(&zsStream);
	ret = ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;

    // Wait for the write thread to finish
	g_thread_join(WriteThread);

	if (ret == Z_OK)
		return true;
	return false;
}



#define READ_BUF_SIZE  2097152
bool Copy_no_compression (TImageStore *Source, TImageStore *Target)
{
char                *ReadBuffer = NULL;
unsigned long       BytesRead = 1,
                    BytesWritten = 0;
unsigned long       ReadBufSize = 0;
unsigned char       FailCounter = 0;
#ifdef USE_FIFO_BUFFER
GError              *error = NULL;
#endif


    // Start the write thread
	if( (WriteThread = g_thread_create((GThreadFunc)WriteThreadProcess, (void *)Target, TRUE, &error)) == NULL)
	{
		printf("Thread create failed: %s!!\n", error->message );
		g_error_free ( error ) ;
        g_mutex_lock (ProcessInfoMutex);
        Info.WriteThreadError = ERROR_WRITETHREAD_FAIL;
        g_mutex_unlock (ProcessInfoMutex);
        CopyThreadAbort = true;
        return false;
	}

    ReadBufSize	= READ_BUF_SIZE;

	ReadBuffer = new char[READ_BUF_SIZE+300];

	while (BytesRead > 0 && !CopyThreadAbort){

        #ifdef USE_FIFO_BUFFER
		if (!Source->Read((void*)ReadBuffer, ReadBufSize, &BytesRead)){
        	if (Source->getType () == isDrive)
                ReadBufSize	= Source->GetBytesPerSector ();
            else
                ReadBufSize	= 512;
            FailCounter++;
            if (FailCounter > 2){
                g_mutex_lock (ProcessInfoMutex);
                Info.ReadThreadError = ERROR_READTHREAD_FAIL;
                g_mutex_unlock (ProcessInfoMutex);
                CopyThreadAbort = true;
                return false;
            }
            continue;
        }

        while (!FifoBuffer.AddData(ReadBuffer, BytesRead) && !CopyThreadAbort)
            g_thread_yield();

    	g_mutex_lock (ProcessInfoMutex);
		Info.BytesRead += BytesRead;
		g_mutex_unlock (ProcessInfoMutex);

        #else

		if (!Source->Read((void*)ReadBuffer, ReadBufSize, &BytesRead)){
        	if (Source->getType () == isDrive)
                ReadBufSize	= Source->GetBytesPerSector ();
            else
                ReadBufSize	= 512;
            FailCounter++;
            if (FailCounter > 2){
                g_mutex_lock (ProcessInfoMutex);
                Info.ReadThreadError = ERROR_READTHREAD_FAIL;
                g_mutex_unlock (ProcessInfoMutex);
                CopyThreadAbort = true;
                return false;
            }
            continue;
        }

		printf ("Bytes read = %i\n", BytesRead);

		if (BytesRead > 0){
			Target->Write(ReadBuffer, BytesRead, &BytesWritten);
			if (BytesRead != BytesWritten){
                g_mutex_lock (ProcessInfoMutex);
                Info.WriteThreadError = ERROR_WRITETHREAD_FAIL;
                g_mutex_unlock (ProcessInfoMutex)
                CopyThreadAbort = true;
				return false;
			}
			g_mutex_lock (ProcessInfoMutex);
			Info.BytesRead += BytesRead;
			Info.BytesWritten += BytesWritten;
			g_mutex_unlock (ProcessInfoMutex);
		}
        #endif
	}

	if (ReadBuffer != NULL)
		delete[] ReadBuffer;

    FifoBuffer.SetWriteFinished ();

	g_thread_join(WriteThread);

	return true;
}

//
// The actual copying thread
// WARNING: there should not be any gtk ui code in here !!!!
//
void *CopyData(void *args)
{
	TImageStore *SourceImage=NULL, *TargetImage=NULL;
	char *buffer = NULL;
	TCopyData *Data;
	int ReadBufSize = 0;

    FifoBuffer.Empty();

	// Get the arguments
	Data = (TCopyData *)args;

	SourceImage = new TImageStore( Data->SourceMediaType, Data->SourceDir.c_str() );

	if (Data->DestinationMediaType == isFile)
		TargetImage = new TImageStore( Data->DestinationMediaType,  Data->Destination.c_str(), true );
	else
		TargetImage = new TImageStore( Data->DestinationMediaType,  Data->Destination.c_str(), true );

	g_mutex_lock (ProcessInfoMutex);
	Info.BytesRead			= 0;
	Info.BytesWritten		= 0;
	Info.TotalSizeInBytes	= SourceImage->GetTotalSizeInBytes ();
	Info.NumbSectors		= SourceImage->GetNumbSectors ();
	Info.BytesPerSector		= SourceImage->GetBytesPerSector ();
    Info.WriteThreadError   = ERROR_READTHREAD_NONE;
    Info.ReadThreadError    = ERROR_WRITETHREAD_NONE;
	if (Data->SourceMediaType == isDrive)
		ReadBufSize			= Info.BytesPerSector;
	else
		ReadBufSize				= 512;
	g_mutex_unlock (ProcessInfoMutex);

	buffer = new char[ReadBufSize+3];

	if (SourceImage->getCompressionFormat() == cfNone || Data->InflateSource == false){
		// The source is not compressed!
		if ( Data->CompressionMethod == Z_BEST_SPEED || Data->CompressionMethod == Z_BEST_COMPRESSION ){
			//Z_BEST_SPEED
			if (!Copy_compressTargetDataGzip (SourceImage, TargetImage, Data->CompressionMethod)){
				//alert ("Error: compress failed");
            }
		}else{
			if (!Copy_no_compression (SourceImage, TargetImage)){
				//alert ("Error: copy failed");
            }
		}
	}else{
		switch (SourceImage->getCompressionFormat()){
			case cfGzipStream:
			case cfGzipChunk:
				if (!Copy_decompressSourceDataGzip (SourceImage, TargetImage)){
					//alert ("Error: decompress failed");
                }
				break;
			case cfBzip2Stream:
			case cfBzip2Chunk:
				break;
			default:
				break;
		}

	}

	CopyThreadDone = true;

	if (buffer != NULL)
		delete[] buffer;

	if (TargetImage != NULL)
		delete TargetImage;

	if (SourceImage != NULL)
	   delete SourceImage;

	return NULL;
}

//
// Handle updating the progress bar and other statistics
//
bool handle_update_progress ()
{
std::string	MbString;
time_t		Start, Last;

	Last = Start = time(0);
	while (!CopyThreadDone){
		g_mutex_lock (ProcessInfoMutex);

		// Update the input size label
		//printf ("Input size = %llu\n", Info.TotalSizeInBytes);
		MbString = "Input size: ";
		MbString += MakeByteLabel(Info.TotalSizeInBytes);
		gtk_label_set_label((GtkLabel *)StatusInputSizeLabel, MbString.c_str());

        if (Info.WriteThreadError != ERROR_READTHREAD_NONE)
            alert ("Write thread error");
        if (Info.ReadThreadError != ERROR_WRITETHREAD_NONE)
            alert ("Read thread error");

		if ((time(0) - Last) > 0.2){

			Last = time(0);

			// Update the bytes read label
			MbString = "Bytes read:\t\t";
			MbString += MakeByteLabel(Info.BytesRead);
			gtk_label_set_label((GtkLabel *)StatusBytesReadLabel, MbString.c_str());

			// Update the bytes written label
			MbString = "Bytes written:\t\t";
			MbString += MakeByteLabel(Info.BytesWritten);
			gtk_label_set_label((GtkLabel *)StatusBytesWrittenLabel, MbString.c_str());

			// Update the avergate speed label
			MbString = "Average speed:\t";
			if ((time(0)-Start) > 0)
				MbString += MakeByteLabel(Info.BytesWritten/(time(0)-Start));
			MbString += "/s";
			gtk_label_set_label((GtkLabel *)StatusAverageSpeedLabel, MbString.c_str());
		}

		if (Info.BytesRead > 0){
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pbar), (float)(((long double)Info.BytesRead/(long double)Info.TotalSizeInBytes)));
		}else
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pbar), 0);

		g_mutex_unlock (ProcessInfoMutex);

		if (gtk_events_pending()){
			gtk_main_iteration(); // Handle unprocessed GTK events
		}else{
			g_thread_yield();     // Yield prcessing time
		}
	}
	g_thread_join(CopyThread);

	gtk_widget_set_sensitive (CancelButton, false);

	g_mutex_lock (ProcessInfoMutex);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pbar), (float)(((double)Info.BytesRead/(double)Info.TotalSizeInBytes)));
	//printf ("TotalSize = %li, BytesRead = %li, Progress = %.8f\n", (long)Info.TotalSizeInBytes, (long)Info.BytesWritten, (float)((double)Info.BytesWritten/(double)Info.TotalSizeInBytes));

	// Update the input size label
	//printf ("Input size = %llu\n", Info.TotalSizeInBytes);
	MbString = "Input size: ";
	MbString += MakeByteLabel(Info.TotalSizeInBytes);
	gtk_label_set_label((GtkLabel *)StatusInputSizeLabel, MbString.c_str() /*InputSizeString*/);

	// Update the bytes read label
	MbString = "Bytes read:\t\t";
	MbString += MakeByteLabel(Info.BytesRead);
	gtk_label_set_label((GtkLabel *)StatusBytesReadLabel, MbString.c_str() /*InputSizeString*/);

	// Update the bytes written label
	MbString = "Bytes written:\t\t";
	MbString += MakeByteLabel(Info.BytesWritten);
	gtk_label_set_label((GtkLabel *)StatusBytesWrittenLabel, MbString.c_str() /*InputSizeString*/);


    if (Info.WriteThreadError != ERROR_READTHREAD_NONE)
        alert ("Write thread error");
    if (Info.ReadThreadError != ERROR_WRITETHREAD_NONE)
        alert ("Read thread error");

	g_mutex_unlock (ProcessInfoMutex);
#ifndef _WIN32
	sync();
#endif

	return TRUE;
}

//
// Do all pre copy checks and start the copy thread
//
bool handle_copy (void)
{
GError		*error = NULL;
std::string	InputPathLabelString,
		OutputPathLabelString;

	CopyThreadAbort = false;

	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pbar), 0);

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(InputFileRadioButton))){
		data.SourceMediaType	= isFile;
		data.SourceDir		= gtk_entry_get_text (GTK_ENTRY (InputFileEntry));

		if (data.SourceDir.size() == 0){
			alert ("Error: incorrect input filename");
			return FALSE;
		}

	}else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(InputDriveRadioButton))){
		data.SourceMediaType	= isDrive;
		std::string DriveName	= gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (InputCobobox)->entry));
		data.SourceDir		= DriveList->GetDrivePath (DriveName);
		//data.SourceDir		= gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (InputCobobox)->entry)); //DriveList->GetDrivePath ((unsigned int) (gtk_combo_box_get_active ((GtkComboBox*)InputCobobox))); //
	}else{
		alert ("Error: unknown media type");
		return FALSE;
	}


	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(OutputFileRadioButton))){
		data.DestinationMediaType	= isFile;
		data.Destination		= gtk_entry_get_text (GTK_ENTRY (OutputFileEntry));

		if (data.Destination.size() == 0){
			alert ("Error: incorrect output filename");
			return FALSE;
		}

		// Check if a file is overwritten and ask if this is allowed if needed :)
		if( g_file_test(data.Destination.c_str(), G_FILE_TEST_EXISTS) ){
			GtkWidget *msgdlg	= NULL;
			msgdlg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_YES_NO, "File already exists, do you want to overwrite?");
			if (gtk_dialog_run(GTK_DIALOG(msgdlg)) == GTK_RESPONSE_NO){
				gtk_widget_destroy(msgdlg);
#ifdef USE_ALERT
				alert ("Fail");
#endif
				return FALSE;
			}
			gtk_widget_destroy(msgdlg);
#if 0
			DoEvents ();
#endif
		}

		// Check if the output file is not a directory
		if (g_file_test(data.Destination.c_str(), G_FILE_TEST_IS_DIR) ){
#ifdef USE_ALERT
			alert ("Error: output file is a directory");
#endif
			return FALSE;
		}

	}else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(OutputDriveRadioButton))){
		data.DestinationMediaType	= isDrive;
		std::string DriveName		= gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (OutputCobobox)->entry));
		data.Destination		= DriveList->GetDrivePath (DriveName);
		//data.Destination		= gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (OutputCobobox)->entry)); //DriveList->GetDrivePath ((unsigned int) (gtk_combo_box_get_active ((GtkComboBox*)OutputCobobox)));//
	}else{
#ifdef USE_ALERT
		alert ("Error: unknown media type");
#endif
		return FALSE;
	}

	// Setup the input and output path labels
	InputPathLabelString	= "Input:\t\t\t";
	InputPathLabelString	+= data.SourceDir;
	OutputPathLabelString	= "Output:\t\t\t";
	OutputPathLabelString	+= data.Destination;
	gtk_label_set_label((GtkLabel *)StatusInputPathLabel, InputPathLabelString.c_str());
	gtk_label_set_label((GtkLabel *)StatusOutputPathLabel, OutputPathLabelString.c_str());

	// Show the Status frame
	gtk_widget_show (StatusFrame);
	gtk_widget_hide (InputFrame);
	gtk_widget_hide (ProcessingFrame);
	gtk_widget_hide (OutputFrame);
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pbar), 0);

	#if 0
	// Handle ui events
	DoEvents ();
	#endif

	// Setup the args for the copy thread

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(DeCompressSourceRadioButton)))
		data.InflateSource	= true;
	else
		data.InflateSource	= false;

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CompressImageRadioButton))){
		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GzipFastRadioButton)))
			data.CompressionMethod	= Z_BEST_SPEED;
		else
			data.CompressionMethod	= Z_BEST_COMPRESSION;
	}else{
		data.CompressionMethod		= 0;
	}

	// Setup the args for the progress status display
	CopyThreadDone         = false;
	Info.BytesRead         = 0;
	Info.BytesWritten      = 0;
    Info.WriteThreadError  = ERROR_READTHREAD_NONE;
    Info.ReadThreadError   = ERROR_WRITETHREAD_NONE;

	// Start the copy thread
	if( (CopyThread = g_thread_create((GThreadFunc)CopyData, (void *)&data, TRUE, &error)) == NULL)
	{
		printf("Thread create failed: %s!!\n", error->message );
		g_error_free ( error ) ;
	}

        return handle_update_progress ();
}

