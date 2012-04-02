#include <stdlib.h>	// atoi ()
#include <gtk/gtk.h>
#include <string.h>
#include <fstream>

#include "../alert/alert.h"
#include "../SelfImage_TImageStore.h"
#include "../SelfImage_Utility.h"
#include "../ReadThread.h"
#include "../WriteThread.h"

std::string LastInputFileName, LastOutputFileName;

extern TDisplayState DisplayState;
extern GtkWidget *OutputFileEntry;
extern GtkWidget *InputFileEntry;
extern GtkWidget *OkeButton, *CancelButton, *HelpButton;
extern GtkWidget *InputFrame;
extern GtkWidget *ProcessingFrame;
extern GtkWidget *OutputFrame;
extern GtkWidget *StatusFrame;
extern GtkWidget *InputFileRadioButton;
extern GtkWidget *InputDriveRadioButton;
extern GtkWidget *InputFileSelectButton;
extern GtkWidget *OutputFileRadioButton;
extern GtkWidget *OutputDriveRadioButton;
extern GtkWidget *OutputFileSelectButton;
extern GtkWidget *InputCobobox;
extern GtkWidget *OutputCobobox;
extern GtkWidget *DeCompressSourceRadioButton;
extern GtkWidget *CompressImageRadioButton;
extern GtkWidget *CompressionNoneRadioButton;
extern GtkWidget *GzipFastRadioButton;
extern GtkWidget *GzipBestRadioButton;
extern GtkWidget *InputSizeLabel;
extern GtkWidget *OutputSizeLabel;
extern GtkWidget *InputTextLabel;
extern GtkWidget *OutputTextLabel;
extern GtkWidget *StatusResultLabel;

extern bool CopyThreadDone;


std::string OrgFileName;

extern TReadThread             ReadThread;
extern TWriteThread            WriteThread;



extern bool handle_copy (void);
extern void InputFileSelectOkButton( GtkWidget *widget, GtkFileSelection *fs );
extern void OutputFileSelectOkButton( GtkWidget *widget, GtkFileSelection *fs );

void SaveSettings (void)
{
FILE *SettingsFile, *TempFile;
char FileName[255];

    SettingsFile = fopen ("./settings.dat", "w");
    if (SettingsFile!=NULL){
        FileName[0] = '\0';
        std::string TempString;
        if (LastInputFileName.size() > 0){
            TempFile = fopen (LastInputFileName.c_str(), "r");
            if (TempFile != NULL){
                TempString = "InputFileName = " + LastInputFileName + "\n";
                fputs (TempString.c_str(), SettingsFile);
                fclose (TempFile);
            }
        }
        if (LastOutputFileName.size() > 0){
            TempString = "OutputFileName = " + LastOutputFileName + "\n";
            fputs (TempString.c_str(), SettingsFile);
            fclose (SettingsFile);
        }
    }
}

/* Respond to a button-press by posting a menu passed in as widget.
 *
 * Note that the "widget" argument is the menu being posted, NOT
 * the button that was pressed.
 */
gboolean ok_button_press( GtkWidget *widget, GdkEvent *event )
{
bool status;
	if (event->type == GDK_BUTTON_PRESS) {
		switch (DisplayState){
			case MAIN:
                if ( GTK_IS_WIDGET (OkeButton)){
				    //gtk_widget_set_sensitive (OkeButton, false);
				    //gtk_widget_set_sensitive (OkeButton, true);
                }
				DisplayState = COPY_STATUS;
  				gtk_widget_hide (StatusResultLabel);
				status = handle_copy ();
                SaveSettings ();

				return status;
				break;
			case COPY_STATUS:
				// Show the main window again
				DisplayState = MAIN;
                if ( GTK_IS_WIDGET (StatusFrame) && GTK_IS_WIDGET (InputFrame) && GTK_IS_WIDGET (ProcessingFrame) && GTK_IS_WIDGET (OutputFrame)){
    				gtk_widget_hide (StatusFrame);
                    gtk_widget_show (InputFrame);
				    gtk_widget_show (ProcessingFrame);
				    gtk_widget_show (OutputFrame);
				    gtk_widget_set_sensitive (CancelButton, true);
				    gtk_widget_set_sensitive (OkeButton, true);
                }
                SaveSettings ();
				break;
		}
	}

	/* Tell calling code that we have not handled this event; pass it on. */
	return FALSE;
}

gboolean cancel_button_press( GtkWidget *widget, GdkEvent *event )
{
	if (event->type == GDK_BUTTON_PRESS) {
		switch (DisplayState){
			case MAIN:
                SaveSettings ();
				exit(0);
				break;
			case COPY_STATUS:
				gtk_widget_set_sensitive (OkeButton, true);
				ReadThread.Abort();
                WriteThread.Abort();
				DisplayState = MAIN;
                if ( GTK_IS_WIDGET (StatusFrame) && GTK_IS_WIDGET (InputFrame) && GTK_IS_WIDGET (ProcessingFrame) && GTK_IS_WIDGET (OutputFrame)){
				    gtk_widget_hide (StatusFrame);
				    gtk_widget_show (InputFrame);
				    gtk_widget_show (ProcessingFrame);
				    gtk_widget_show (OutputFrame);
                }
                SaveSettings ();
				break;
		}
	}
	
	gtk_widget_set_sensitive (CancelButton, true);

	/* Tell calling code that we have not handled this event; pass it on. */
	return FALSE;
}

gboolean CompressionRadioButton_callback( GtkWidget *widget, GdkEvent *event )
{
std::string	filename,
		Extension;

    if ( GTK_IS_WIDGET (CompressionNoneRadioButton) && GTK_IS_WIDGET (CompressImageRadioButton) && GTK_IS_WIDGET (GzipFastRadioButton) && GTK_IS_WIDGET (GzipBestRadioButton)){ 
    	if (	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CompressionNoneRadioButton)) || 
    		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(DeCompressSourceRadioButton))){
		
    		gtk_widget_set_sensitive (GzipFastRadioButton, false);
    		gtk_widget_set_sensitive (GzipBestRadioButton, false);
	
    		filename	= gtk_entry_get_text (GTK_ENTRY (OutputFileEntry));
    		Extension	= ExtractFileExt(filename);
    		if (Extension	== ".gz" && filename.size() > 0){
    			filename.erase (filename.size() - 3,3);
    			gtk_entry_set_text (GTK_ENTRY (OutputFileEntry), filename.c_str());
    		}

            // Check for the img extension
    		Extension	= ExtractFileExt(filename);
            if (Extension != ".img" && filename.size() > 0){
                filename += ".img";
    			gtk_entry_set_text (GTK_ENTRY (OutputFileEntry), filename.c_str());
            }
            LastOutputFileName = filename;
    	}
    	if (	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CompressImageRadioButton)) &&
    		(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GzipFastRadioButton)) ||
    		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GzipBestRadioButton)))){
		
    		gtk_widget_set_sensitive (GzipFastRadioButton, true);
    		gtk_widget_set_sensitive (GzipBestRadioButton, true);
		
    		filename	= gtk_entry_get_text (GTK_ENTRY (OutputFileEntry));
    		Extension	= ExtractFileExt(filename);

            if (Extension != ".img" && Extension != ".gz" && filename.size() > 0){
                filename += ".img";
    			gtk_entry_set_text (GTK_ENTRY (OutputFileEntry), filename.c_str());
            }

    		if (Extension	!= ".gz" && filename.size() > 0){
    			filename += ".gz";
    			gtk_entry_set_text (GTK_ENTRY (OutputFileEntry), filename.c_str());
    		}
            LastOutputFileName = filename;
	   }
    }
	return true;

}


gboolean InputRadioButttons_callback( GtkWidget *widget, GdkEvent *event )
{
    if ( GTK_IS_WIDGET (InputFileRadioButton) && GTK_IS_WIDGET (InputDriveRadioButton) && GTK_IS_WIDGET (DeCompressSourceRadioButton)){ 
    	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(InputFileRadioButton))){
    		gtk_widget_set_sensitive (DeCompressSourceRadioButton, true);
    		if ( GTK_IS_WIDGET (InputFileEntry) && GTK_IS_WIDGET (InputFileEntry) && GTK_IS_WIDGET (InputFileEntry)){
    			gtk_widget_show ( InputFileEntry );
    			gtk_widget_show ( InputFileSelectButton );
    			gtk_widget_hide ( InputCobobox );
                if (GTK_IS_WIDGET (InputTextLabel) && GTK_IS_WIDGET (InputSizeLabel)){
//                	gtk_widget_hide ( InputSizeLabel );
//                	gtk_widget_hide ( InputTextLabel );
                    gtk_label_set_label((GtkLabel *)InputSizeLabel, "0 B");
                }
    			//gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (OutputDriveRadioButton), TRUE);

                /* Create a new file selection widget */
                GtkWidget *filew = gtk_file_selection_new ("Select input file");
                gtk_file_selection_set_filename(GTK_FILE_SELECTION (filew), LastInputFileName.c_str());
                g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button), "clicked", G_CALLBACK (InputFileSelectOkButton), (gpointer) filew);
                g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (filew));
                gtk_widget_show (filew);
    		}
    	}else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(InputDriveRadioButton))){
    		if (GTK_IS_TOGGLE_BUTTON (CompressImageRadioButton) && GTK_IS_TOGGLE_BUTTON (DeCompressSourceRadioButton)){
    			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (CompressImageRadioButton), TRUE);
    			gtk_widget_set_sensitive (DeCompressSourceRadioButton, false);
    			//gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (OutputFileRadioButton), TRUE);
    		}
    		if (GTK_IS_WIDGET (InputFileEntry) && GTK_IS_WIDGET (InputFileEntry) && GTK_IS_WIDGET (InputFileEntry)){
    			gtk_widget_show ( InputCobobox );
    			gtk_widget_hide ( InputFileSelectButton );
    			gtk_widget_hide ( InputFileEntry );
    		}
            if (GTK_IS_WIDGET (InputTextLabel) && GTK_IS_WIDGET (InputSizeLabel)){
              	gtk_widget_show ( InputSizeLabel );
               	gtk_widget_show ( InputTextLabel );
            }
        }
	}
	
	return true;

}

gboolean OutputRadioButttons_callback( GtkWidget *widget, GdkEvent *event )
{
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(OutputFileRadioButton))){
		if (GTK_IS_WIDGET (OutputFileEntry) && GTK_IS_WIDGET (OutputFileSelectButton) && GTK_IS_WIDGET (OutputCobobox)){
			gtk_widget_show ( OutputFileEntry );
			gtk_widget_show ( OutputFileSelectButton );
			gtk_widget_hide ( OutputCobobox );
            if (GTK_IS_WIDGET (OutputSizeLabel) && GTK_IS_WIDGET (OutputTextLabel)){
            	gtk_widget_hide ( OutputSizeLabel );
        	    gtk_widget_hide ( OutputTextLabel );
            }

            /* Create a new file selection widget */
            GtkWidget *filew = gtk_file_selection_new ("Select ouput file");
            gtk_file_selection_set_filename(GTK_FILE_SELECTION (filew), LastOutputFileName.c_str());
            g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button), "clicked", G_CALLBACK (OutputFileSelectOkButton), (gpointer) filew);
            g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (filew));
            gtk_widget_show (filew);
		}
	}else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(OutputDriveRadioButton))){
		if (GTK_IS_WIDGET (OutputCobobox) && GTK_IS_WIDGET (OutputFileSelectButton) && GTK_IS_WIDGET (OutputFileEntry)){
			gtk_widget_show ( OutputCobobox );
			gtk_widget_hide ( OutputFileSelectButton );
			gtk_widget_hide ( OutputFileEntry );

		}
        if (GTK_IS_WIDGET (OutputSizeLabel) && GTK_IS_WIDGET (OutputTextLabel)){
         	gtk_widget_show ( OutputSizeLabel );
           	gtk_widget_show ( OutputTextLabel );
        }
	}
	
	return true;

}

/* Get the selected filename and store it in the inputfilentry */
void OutputFileSelectOkButton( GtkWidget *widget, GtkFileSelection *fs )
{
GtkWidget *FileWindow = NULL;
std::string filename;
int Length;
std::string Extension;

	// Store the filename in the inputfilentry field
	filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs));
	
	Extension = ExtractFileExt(filename);
	
	if (	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CompressImageRadioButton))){
		if (	(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GzipFastRadioButton)) ||
			gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GzipBestRadioButton)))){

            if (Extension != ".img" && Extension != ".gz" && filename.size() > 0){
                filename += ".img";
    			gtk_entry_set_text (GTK_ENTRY (OutputFileEntry), filename.c_str());
            }

            if (Extension != ".gz" && filename.size() > 0){
    			filename += ".gz";
            }
            LastOutputFileName = filename;
		}
	}else{
        if (Extension != ".img" && Extension != ".gz" && filename.size() > 0){
			filename += ".img";
        }
        LastOutputFileName = filename;
    }
    SaveSettings ();	

	if( !g_file_test(filename.c_str(), G_FILE_TEST_IS_DIR) ){
		Length = strlen (filename.c_str());
		gtk_entry_set_text (GTK_ENTRY (OutputFileEntry), filename.c_str());
		gtk_editable_set_position (GTK_EDITABLE (OutputFileEntry), Length);
	}

	/* Let's get the parent window */
	for (FileWindow = widget; !GTK_IS_WINDOW (FileWindow); FileWindow = FileWindow->parent);

	// Destroy the parent window :)
	if (GTK_IS_WINDOW (FileWindow))
		gtk_widget_destroy (FileWindow);
}

/* Get the selected filename and store it in the inputfilentry */
void InputFileSelectOkButton( GtkWidget *widget, GtkFileSelection *fs )
{
GtkWidget	*FileWindow = NULL;
const gchar	*filename;
int		Length;
std::string	Extension;

    /* Let's get the parent window */
    for (FileWindow = widget; !GTK_IS_WINDOW (FileWindow); FileWindow = FileWindow->parent);	


	// Store the filename in the inputfilentry field
	filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs));
	if( g_file_test(filename, G_FILE_TEST_EXISTS) && !g_file_test(filename, G_FILE_TEST_IS_DIR) ){
		Length = strlen (filename);
		gtk_entry_set_text (GTK_ENTRY (InputFileEntry), filename);
		gtk_editable_set_position (GTK_EDITABLE (InputFileEntry), Length);
	}else{
    	if (GTK_IS_WINDOW (FileWindow))
	       gtk_widget_destroy (FileWindow);
        alert ("Invalid file name");
        return;
    }
	
	Extension = ExtractFileExt(filename);
	if (Extension == ".gz") {
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (DeCompressSourceRadioButton), TRUE);
	}else if (Extension == ".gz"){
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (CompressionNoneRadioButton /*CompressImageRadioButton*/), TRUE);
	}
//	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (OutputDriveRadioButton /*CompressImageRadioButton*/), TRUE);

	if (GTK_IS_WINDOW (FileWindow))
		gtk_widget_destroy (FileWindow);

    LastInputFileName = filename;
    SaveSettings ();

    // Set the input file size label
	std::string InputSizeText = MakeByteLabel(GetFileSizeByName(LastInputFileName));
    gtk_label_set_label((GtkLabel *)InputSizeLabel, InputSizeText.c_str());
}


gboolean ButtonCallback( GtkWidget *widget, gpointer data )
{
GtkWidget *filew = NULL;

	
	if (strcmp ((char *) data, "InputFileSelect button") == 0){

		/* Create a new file selection widget */
		filew = gtk_file_selection_new ("Select input file");

        /* Set the filename to select */
        gtk_file_selection_set_filename(GTK_FILE_SELECTION (filew), LastInputFileName.c_str());
	
		/* Connect the ok_button to file_ok_sel function */
		g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button), "clicked", G_CALLBACK (InputFileSelectOkButton), (gpointer) filew);
	
		/* Connect the cancel_button to destroy the widget */
		g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (filew));
	
		gtk_widget_show (filew);

	}else if (strcmp ((char *) data, "OutputFileSelect button") == 0){

		/* Create a new file selection widget */
		filew = gtk_file_selection_new ("Select output file");

        /* Set the filename to select */
        gtk_file_selection_set_filename(GTK_FILE_SELECTION (filew), LastOutputFileName.c_str());
	
		/* Connect the ok_button to file_ok_sel function */
		g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button), "clicked", G_CALLBACK (OutputFileSelectOkButton), (gpointer) filew);
	
		/* Connect the cancel_button to destroy the widget */
		g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button), "clicked", G_CALLBACK (gtk_widget_destroy), G_OBJECT (filew));
	
		gtk_widget_show (filew);
	
	}
	
	return true;
}

