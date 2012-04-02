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
#include "ReadThread.h"
#include "WriteThread.h"

// zlib includes
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zlib.h"

#define USE_ALERT
#define USE_FIFO_BUFFER


TFifoBufferClass    FifoBuffer;


// Local var def's
GThread			 *CopyThread;
TCopyData        data;
TProgressInfo	Info;
bool		CopyThreadDone;
bool		CopyThreadAbort;
bool		DeflateInit = true;

// Write and read threads
TReadThread             ReadThread;
TWriteThread            WriteThread;


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
extern GtkWidget *OkeButton;
extern GtkWidget *pbar;
extern GtkWidget *InputFrame;
extern GtkWidget *ProcessingFrame;
extern GtkWidget *OutputFrame;
extern GtkWidget *StatusFrame;
extern GtkWidget *StatusInputSizeLabel;
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
extern GtkWidget *StatusResultLabel;

extern TDriveList *DriveList;



//
// The actual copying thread
// WARNING: there should not be any gtk ui code in here !!!!
//
void *CopyData(void *args)
{
TCopyData               *Data;

	// Get the arguments
	Data = (TCopyData *)args;

	FifoBuffer.Empty();

	Data->ImgBuffer    = &FifoBuffer;

	ReadThread.Start(Data);
	WriteThread.Start (Data);

	ReadThread.join();
	WriteThread.join();

	CopyThreadDone = true;

	return NULL;
}


//
// Handle updating the progress bar and other statistics
//
bool handle_update_progress ()
{
std::string	MbString;
time_t		Start, Last;
GdkColor    color;

	Last = Start = time(0);

	while (!CopyThreadDone){

		if ((time(0) - Last) > 0.4){

			Last = time(0);

			// Update the input size label
			//printf ("Input size = %llu\n", Info.TotalSizeInBytes);
			MbString = "Input size: ";
			MbString += MakeByteLabel(ReadThread.GetTotalSizeInBytes());
			gtk_label_set_label((GtkLabel *)StatusInputSizeLabel, MbString.c_str());

			if (WriteThread.GetLastError () != ERROR_WRITETHREAD_NONE){
				//alert ("Write thread error");
				ReadThread.Abort();
			}

			if (ReadThread.GetLastError () != ERROR_READTHREAD_NONE){
				//alert ("Read thread error");
				WriteThread.Abort();
			}

			// Update the bytes read label
			MbString = "Bytes read: \t\t";
			MbString += ReadThread.GetNumbBytesReadString ();
			gtk_label_set_label((GtkLabel *)StatusBytesReadLabel, MbString.c_str());

			// Update the bytes written label
			MbString = "Bytes written: \t\t";
			MbString += WriteThread.GetNumbBytesWrittenString ();
			gtk_label_set_label((GtkLabel *)StatusBytesWrittenLabel, MbString.c_str());

			// Update the avergate speed label
			MbString = "Average speed:\t\t";
			if ((time(0)-Start) > 0)
				MbString += WriteThread.GetAverageSpeedString ();
			gtk_label_set_label((GtkLabel *)StatusAverageSpeedLabel, MbString.c_str());

			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pbar), (float)ReadThread.GetProgress () );

		}

		if (gtk_events_pending()){
			gtk_main_iteration(); // Handle unprocessed GTK events
		}else{
			g_thread_yield();     // Yield processing time
		}
	}
	g_thread_join(CopyThread);

	gtk_widget_set_sensitive (CancelButton, false);

	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pbar), (float)ReadThread.GetProgress () );

	// Update the bytes read label
	MbString = "Bytes read: \t\t";
	MbString += ReadThread.GetNumbBytesReadString ();
	gtk_label_set_label((GtkLabel *)StatusBytesReadLabel, MbString.c_str());

	// Update the bytes written label
	MbString = "Bytes written: \t\t";
	MbString += WriteThread.GetNumbBytesWrittenString ();
	gtk_label_set_label((GtkLabel *)StatusBytesWrittenLabel, MbString.c_str());

#ifndef _WIN32
	sync();
#endif

    if (WriteThread.GetLastError () == ERROR_WRITETHREAD_NONE && ReadThread.GetLastError () == ERROR_READTHREAD_NONE){
        // Copying was succesfull
        gtk_label_set_label((GtkLabel *)StatusResultLabel, "Copy done succesfully");
        gdk_color_parse ("#2C9014", &color);
    }else if (CopyThreadAbort){
        // Copying was aborted
        gtk_label_set_label((GtkLabel *)StatusResultLabel, "Copy aborted");
        gdk_color_parse ("#FF0000", &color);
    }else if (ReadThread.GetLastError () == ERROR_READTHREAD_UNSUPPORTED_FORMAT){
        // Selfimage compression format (unhandled)
        gtk_label_set_label((GtkLabel *)StatusResultLabel, "ERROR: unsupported compression format");
        gdk_color_parse ("#FF0000", &color);
    }else{
        // Copying had errors
        if (ReadThread.GetLastError () != ERROR_WRITETHREAD_NONE){
            gtk_label_set_label((GtkLabel *)StatusResultLabel, "READ ERROR");
        }else{
            gtk_label_set_label((GtkLabel *)StatusResultLabel, "WRITE ERROR");
        }
        gdk_color_parse ("#FF0000", &color);
    }
    gtk_widget_modify_fg (StatusResultLabel, GTK_STATE_NORMAL, &color);
   	gtk_widget_show (StatusResultLabel);

	gtk_widget_set_sensitive (OkeButton, true);

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
        data.SourceDir		      = DriveList->GetDrivePath (gtk_combo_box_get_active (GTK_COMBO_BOX(InputCobobox)));
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
				return FALSE;
			}
			gtk_widget_destroy(msgdlg);
		}

		// Check if the output file is not a directory
		if (g_file_test(data.Destination.c_str(), G_FILE_TEST_IS_DIR) ){
			alert ("Error: output file is a directory");
			return FALSE;
		}

	}else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(OutputDriveRadioButton))){
		data.DestinationMediaType	= isDrive;
		data.Destination	         = DriveList->GetDrivePath (gtk_combo_box_get_active (GTK_COMBO_BOX(OutputCobobox)));

        // Ask if the data on this drive may be overwritten
        GtkWidget *msgdlg	= NULL;
		msgdlg = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_YES_NO, "All data on the destination drive will be lost, do you want to continue?");
		if (gtk_dialog_run(GTK_DIALOG(msgdlg)) == GTK_RESPONSE_NO){
			gtk_widget_destroy(msgdlg);
			return FALSE;
		}
		gtk_widget_destroy(msgdlg);
	}else{
		alert ("Error: unknown media type");
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
	Info.WriteThreadError  = ERROR_WRITETHREAD_NONE;
	Info.ReadThreadError   = ERROR_READTHREAD_NONE;

	gtk_widget_set_sensitive (OkeButton, false);

	// Start the copy thread
	if( (CopyThread = g_thread_create((GThreadFunc)CopyData, (void *)&data, TRUE, &error)) == NULL)
	{
		printf("Thread create failed: %s!!\n", error->message );
		g_error_free ( error ) ;
	}

    return handle_update_progress ();
}

