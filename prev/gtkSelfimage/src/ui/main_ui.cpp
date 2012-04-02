#include <stdlib.h>	// atoi ()
#include <gtk/gtk.h>
#include <string.h>
#include <fstream>


#include "../alert/alert.h"
#include "../ui/buttons.h"
#include "../ui/combobox.h"
#include "../SelfImage_TImageStore.h"
#include "../DriveList.h"

extern TDriveList	*DriveList;

GtkWidget *InputFrame;
GtkWidget *ProcessingFrame;
GtkWidget *CompressionMethodFrame;
GtkWidget *OutputFrame;
GtkWidget *StatusFrame;
GtkWidget *InputFileEntry;
GtkWidget *OutputFileEntry;
GtkWidget *InputFileRadioButton;
GtkWidget *InputDriveRadioButton;
GtkWidget *InputFileSelectButton;
GtkWidget *OutputFileRadioButton;
GtkWidget *OutputDriveRadioButton;
GtkWidget *OutputFileSelectButton;
GtkWidget *InputCobobox;
GtkWidget *OutputCobobox;
GtkWidget *pbar;
GtkWidget *OkeButton;
GtkWidget *CancelButton;
GtkWidget *HelpButton;
GtkWidget *InputSizeLabel;
GtkWidget *OutputSizeLabel;
GtkWidget *InputTextLabel;
GtkWidget *OutputTextLabel;
GtkWidget *StatusInputSizeLabel;
GtkWidget *StatusInputPathLabel;
GtkWidget *StatusOutputPathLabel;
GtkWidget *StatusBytesWrittenLabel;
GtkWidget *StatusBytesReadLabel;
GtkWidget *StatusAverageSpeedLabel;
GtkWidget *CompressImageRadioButton;
GtkWidget *DeCompressSourceRadioButton;
GtkWidget *CompressionNoneRadioButton;
GtkWidget *GzipFastRadioButton;
GtkWidget *GzipBestRadioButton;
GtkWidget *StatusResultLabel;
GtkWidget *align;


/* Print a string when a menu item is selected */
static void menuitem_response( gchar *string )
{
    printf ("%s\n", string);
    if (strcmp (string, "exit") == 0){
        gtk_main_quit();
    }
}


void Initialize_ui (void)
{
GtkWidget *window;
GtkWidget *menu;
GtkWidget *menu_bar;
GtkWidget *root_menu;
GtkWidget *menu_items;
GtkWidget *vbox_main;
GtkWidget *vbox_input;
GtkWidget *vbox_output;
GtkWidget *vbox_status;
GtkWidget *vbox_processing;
GtkWidget *hbox;
GSList *group;
char buf[128];
int i;
    unsigned int Index;
    std::string InputSizeText;
    std::string OutputSizeText;

std::string DriveName;

	/* init threads */
//	g_thread_init(NULL);

	/* init gtk */
//	gtk_init (&argc, &argv);


#ifndef _WIN32
	/* Check we have root privelages */
	if(getuid())
	{
		alert ("ERROR: This program needs root priv's");
		return;
	}
#endif

	//================================================================================================
	// create a new window
	//================================================================================================

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (GTK_WIDGET (window), 200, 100);
	gtk_window_set_title (GTK_WINDOW (window), "GTK Selfimage");
	g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (gtk_main_quit), NULL);


	//================================================================================================
	// Set the application icon
	//================================================================================================

	/* load the icon for the window; here we just load one, default icon */
	gtk_window_set_default_icon_from_file ("./Images/kcmpartitions.png", NULL);

	//================================================================================================
	// create a menu bar
	//================================================================================================

	/* Init the menu-widget, and remember -- never
	* gtk_show_widget() the menu widget!!
	* This is the menu that holds the menu items, the one that
	* will pop up when you click on the "Root Menu" in the app */
	menu = gtk_menu_new ();

	/* Next we make a little loop that makes three menu-entries for "test-menu".
	* Notice the call to gtk_menu_shell_append.  Here we are adding a list of
	* menu items to our menu.  Normally, we'd also catch the "clicked"
	* signal on each of the menu items and setup a callback for it,
	* but it's omitted here to save space. */

	for (i = 0; i < 3; i++){

		/* Copy the names to the buf. */
        if (i == 2){
    		sprintf (buf, "exit");
        }else{
    		sprintf (buf, "Test-undermenu - %d", i);
        }

		/* Create a new menu-item with a name... */
		menu_items = gtk_menu_item_new_with_label (buf);

		/* ...and add it to the menu. */
		gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_items);

		/* Do something interesting when the menuitem is selected */
		g_signal_connect_swapped (G_OBJECT (menu_items), "activate",  G_CALLBACK (menuitem_response), (gpointer) g_strdup (buf));

		/* Show the widget */
		gtk_widget_show (menu_items);
	}


	/* This is the root menu, and will be the label
	* displayed on the menu bar.  There won't be a signal handler attached,
	* as it only pops up the rest of the menu when pressed. */
	root_menu = gtk_menu_item_new_with_label ("File");

	gtk_widget_show (root_menu);

	/* Now we specify that we want our newly created "menu" to be the menu
	* for the "root menu" */
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (root_menu), menu);


	//================================================================================================
	// Menu bar (box)
	//================================================================================================

        /* A vbox to put a menu and a button in: */
	vbox_main = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), vbox_main);
	gtk_widget_show (vbox_main);

	/* Create a menu-bar to hold the menus and add it to our main window */
	menu_bar = gtk_menu_bar_new ();
	gtk_box_pack_start (GTK_BOX (vbox_main), menu_bar, FALSE, FALSE, 2);
	gtk_widget_show (menu_bar);

	//================================================================================================
	// Input frame
	//================================================================================================

	/* Create the input frame*/
	InputFrame = gtk_frame_new ("Input");
	gtk_container_set_border_width (GTK_CONTAINER (InputFrame), 5);
	gtk_box_pack_start (GTK_BOX (vbox_main), InputFrame, TRUE, TRUE, 0);
	gtk_widget_show (InputFrame);

	/* Create the sub vbox for the input radio buttons etc */
	vbox_input = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (InputFrame), vbox_input);
	gtk_widget_show (vbox_input);

	/* Create the hbox for the input radio buttons etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 1);
	//gtk_box_set_spacing (GTK_BOX (hbox), 10);
	gtk_box_pack_start (GTK_BOX (vbox_input), hbox, TRUE, TRUE, 0);
	gtk_widget_show (hbox);

	/* Create the "File" input radio button */
	InputFileRadioButton = gtk_radio_button_new_with_label (NULL, "File");
	gtk_box_pack_start (GTK_BOX (hbox), InputFileRadioButton, FALSE, TRUE, 0);
	gtk_widget_show (InputFileRadioButton);
	g_signal_connect_swapped(G_OBJECT(InputFileRadioButton), "clicked", G_CALLBACK(InputRadioButttons_callback), (gpointer)"");

	// Create the "Drive" input radio button
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (InputFileRadioButton));
	InputDriveRadioButton = gtk_radio_button_new_with_label (group, "Drive");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (InputDriveRadioButton), TRUE);
	gtk_box_pack_start (GTK_BOX (hbox), InputDriveRadioButton, FALSE, TRUE, 0);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (InputDriveRadioButton), TRUE);
	gtk_widget_show (InputDriveRadioButton);

	/* Create the hbox to hold the input field and the button */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_input), hbox, TRUE, TRUE, 5);
	gtk_widget_show (hbox);

	// Create and display the input cobobox
	InputCobobox = CreateInputCombobox ();
	gtk_box_pack_start (GTK_BOX (hbox), InputCobobox, TRUE, TRUE, 2);
	gtk_widget_show (InputCobobox);

	// Create the file input field
	InputFileEntry = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY (InputFileEntry), 50);
	gtk_entry_set_text (GTK_ENTRY (InputFileEntry), "");
	gtk_box_pack_start (GTK_BOX (hbox), InputFileEntry, TRUE, TRUE, 2);
	gtk_entry_set_editable(GTK_ENTRY (InputFileEntry), FALSE);
	gtk_widget_hide( InputFileEntry );

	// Create the button with the file input field
	InputFileSelectButton = gtk_button_new_with_label ("...");
	g_signal_connect (G_OBJECT (InputFileSelectButton), "clicked", G_CALLBACK (ButtonCallback), (gpointer) "InputFileSelect button");
	gtk_box_pack_start (GTK_BOX (hbox), InputFileSelectButton, FALSE, TRUE, 0);
	gtk_widget_hide (InputFileSelectButton);
	gtk_widget_set_size_request     (InputFileSelectButton, 44, 10);

	/* Create the hbox for the lable etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 1);
	gtk_box_pack_start (GTK_BOX (vbox_input), hbox, TRUE, TRUE, 0);
	gtk_widget_show (hbox);

    /* The input size label */
	InputTextLabel = gtk_label_new("Input size: ");
	gtk_box_pack_start (GTK_BOX (hbox), InputTextLabel, FALSE, TRUE, 2);
	gtk_widget_show (InputTextLabel);

    /* The size label for the input size */
    Index = gtk_combo_box_get_active (GTK_COMBO_BOX(InputCobobox));
    InputSizeText = DriveList->GetDriveSizeString (Index).c_str();
	InputSizeLabel = gtk_label_new(InputSizeText.c_str());
	gtk_box_pack_start (GTK_BOX (hbox), InputSizeLabel, FALSE, TRUE, 2);
    gtk_widget_set_name(InputSizeLabel, "PartSizeLabel2");
	gtk_widget_show (InputSizeLabel);

	//================================================================================================
	// Processing frame
	//================================================================================================

	/* Create the processing frame*/
	ProcessingFrame = gtk_frame_new ("Processing");
	gtk_container_set_border_width (GTK_CONTAINER (ProcessingFrame), 5);
	gtk_box_pack_start (GTK_BOX (vbox_main), ProcessingFrame, TRUE, TRUE, 0);
	gtk_widget_show (ProcessingFrame);

	/* Create the sub vbox for the input radio buttons etc */
	vbox_processing = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (ProcessingFrame), vbox_processing);
	gtk_widget_show (vbox_processing);

	/* Create the "None" radio button */
	CompressionNoneRadioButton = gtk_radio_button_new_with_label (NULL, "None");
	gtk_box_pack_start (GTK_BOX (vbox_processing), CompressionNoneRadioButton, FALSE, TRUE, 7);
	//gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (CompressionNoneRadioButton), TRUE);
	gtk_widget_show (CompressionNoneRadioButton);
	g_signal_connect_swapped(G_OBJECT(CompressionNoneRadioButton), "clicked", G_CALLBACK(CompressionRadioButton_callback), (gpointer)"");

	// Create the "Decompress source" radio button
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (CompressionNoneRadioButton));
	DeCompressSourceRadioButton = gtk_radio_button_new_with_label (group, "Decompress source");
	gtk_box_pack_start (GTK_BOX (vbox_processing), DeCompressSourceRadioButton, FALSE, TRUE, 0);
	gtk_widget_show (DeCompressSourceRadioButton);
	gtk_widget_set_sensitive (DeCompressSourceRadioButton, false);
	g_signal_connect_swapped(G_OBJECT(DeCompressSourceRadioButton), "clicked", G_CALLBACK(CompressionRadioButton_callback), (gpointer)"");

	/* Create the hbox for the lable etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 1);
	gtk_box_pack_start (GTK_BOX (vbox_processing), hbox, TRUE, TRUE, 2);
	gtk_widget_show (hbox);

	// Create the "Compress image" radio button
	CompressImageRadioButton = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (DeCompressSourceRadioButton), "Compress image  ");
	gtk_box_pack_start (GTK_BOX (hbox), CompressImageRadioButton, FALSE, TRUE, 0);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (CompressImageRadioButton), TRUE);
	gtk_widget_show (CompressImageRadioButton);

	/* Create the compression method frame*/
	CompressionMethodFrame = gtk_frame_new ("Compression method");
	gtk_container_set_border_width (GTK_CONTAINER (CompressionMethodFrame), 0);
	gtk_box_pack_start (GTK_BOX (hbox), CompressionMethodFrame, TRUE, TRUE, 5);
	gtk_widget_show (CompressionMethodFrame);

	/* Create the sub hbox for the zipping method radio buttons etc */
	hbox = gtk_hbox_new (FALSE, 5);
	gtk_container_add (GTK_CONTAINER (CompressionMethodFrame), hbox);
	gtk_widget_show (hbox);

	/* Create the "gzip" radio button */
	GzipFastRadioButton = gtk_radio_button_new_with_label (NULL, "gzip (fast)");
	gtk_box_pack_start (GTK_BOX (hbox), GzipFastRadioButton, FALSE, TRUE, 0);
	gtk_widget_show (GzipFastRadioButton);
//	g_signal_connect_swapped(G_OBJECT(CompressionNoneRadioButton), "clicked", G_CALLBACK(CompressionRadioButton_callback), (gpointer)"");

	// Create the "Drive" input radio button
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (GzipFastRadioButton));
	GzipBestRadioButton = gtk_radio_button_new_with_label (group, "gzip (best)");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (GzipBestRadioButton), TRUE);
	gtk_box_pack_start (GTK_BOX (hbox), GzipBestRadioButton, FALSE, TRUE, 0);
	gtk_widget_show (GzipBestRadioButton);


	//================================================================================================
	// Output frame
	//================================================================================================

	/* Create the output frame*/
	OutputFrame = gtk_frame_new ("Output");
	gtk_container_set_border_width (GTK_CONTAINER (OutputFrame), 5);
	gtk_box_pack_start (GTK_BOX (vbox_main), OutputFrame, TRUE, TRUE, 0);
	gtk_widget_show (OutputFrame);

	/* Create the sub vbox for the input radio buttons etc */
	vbox_output = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox_output), 5);
	gtk_container_add (GTK_CONTAINER (OutputFrame), vbox_output);
	gtk_widget_show (vbox_output);

	/* Create the hbox for the input radio buttons etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 1);
	gtk_box_pack_start (GTK_BOX (vbox_output), hbox, TRUE, TRUE, 0);
	gtk_widget_show (hbox);

	/* Create the "File" input radio button */
	OutputFileRadioButton = gtk_radio_button_new_with_label (NULL, "File");
	gtk_box_pack_start (GTK_BOX (hbox), OutputFileRadioButton, FALSE, TRUE, 0);
	gtk_widget_show (OutputFileRadioButton);

	// Create the "Drive" input radio button
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (OutputFileRadioButton));
	OutputDriveRadioButton = gtk_radio_button_new_with_label (group, "Drive");
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (OutputDriveRadioButton), TRUE);
	gtk_box_pack_start (GTK_BOX (hbox), OutputDriveRadioButton, FALSE, TRUE, 0);
	gtk_widget_show (OutputDriveRadioButton);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (OutputFileRadioButton), TRUE);
	g_signal_connect_swapped(G_OBJECT(OutputDriveRadioButton), "clicked", G_CALLBACK(OutputRadioButttons_callback), (gpointer)"");

	/* Create the hbox to hold the input field and the button */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_output), hbox, TRUE, TRUE, 5);
	gtk_widget_show (hbox);

	OutputCobobox = CreateOutputCombobox ();
	//gtk_box_pack_start (GTK_BOX (vbox_output), OutputCobobox, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), OutputCobobox, TRUE, TRUE, 2);
	gtk_widget_hide (OutputCobobox);

	// Create the file input field
	OutputFileEntry = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY (OutputFileEntry), 50);
	gtk_entry_set_text (GTK_ENTRY (OutputFileEntry), "");
	gtk_box_pack_start (GTK_BOX (hbox), OutputFileEntry, TRUE, TRUE, 2);
	gtk_entry_set_editable(GTK_ENTRY (OutputFileEntry), TRUE);
	gtk_widget_show( OutputFileEntry );

	// Create the button with the file input field
	OutputFileSelectButton = gtk_button_new_with_label ("...");
	g_signal_connect (G_OBJECT (OutputFileSelectButton), "clicked", G_CALLBACK (ButtonCallback), (gpointer) "OutputFileSelect button");
	gtk_box_pack_start (GTK_BOX (hbox), OutputFileSelectButton, FALSE, FALSE, 0);
	gtk_widget_show (OutputFileSelectButton);
	gtk_widget_set_size_request     (OutputFileSelectButton, 44, 10);

	/* Create the hbox for the lable etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 1);
	gtk_box_pack_start (GTK_BOX (vbox_output), hbox, TRUE, TRUE, 0);
	gtk_widget_show (hbox);

	OutputTextLabel = gtk_label_new("Input size: ");
	gtk_box_pack_start (GTK_BOX (hbox), OutputTextLabel, FALSE, TRUE, 2);
	gtk_widget_hide (OutputTextLabel);

    Index = gtk_combo_box_get_active (GTK_COMBO_BOX(OutputCobobox));
    OutputSizeText = DriveList->GetDriveSizeString (Index).c_str();
	OutputSizeLabel = gtk_label_new(OutputSizeText.c_str());
	gtk_box_pack_start (GTK_BOX (hbox), OutputSizeLabel, FALSE, TRUE, 2);
    gtk_widget_set_name(OutputSizeLabel, "PartSizeLabel1");
	gtk_widget_hide (OutputSizeLabel);

	//================================================================================================
	//Status (progress) dialog (used while copieing)
	//================================================================================================

	/* Create the progress bar frame*/
	StatusFrame = gtk_frame_new ("Status");
	gtk_container_set_border_width (GTK_CONTAINER (StatusFrame), 5);
	gtk_box_pack_start (GTK_BOX (vbox_main), StatusFrame, TRUE, TRUE, 0);
	gtk_widget_hide (StatusFrame);

	/* Create the sub vbox for the input radio buttons etc */
	vbox_status = gtk_vbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (vbox_status), 5);
	gtk_container_add (GTK_CONTAINER (StatusFrame), vbox_status);
	gtk_widget_show (vbox_status);

	pbar = gtk_progress_bar_new();
	gtk_box_pack_start (GTK_BOX (vbox_status), pbar, FALSE, FALSE, 2);
	gtk_progress_set_format_string(GTK_PROGRESS(pbar), "%p%%");
	gtk_progress_set_show_text(GTK_PROGRESS(pbar), TRUE);
	gtk_widget_show (pbar);

	/* Create the hbox for the lable etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 1);
	gtk_box_pack_start (GTK_BOX (vbox_status), hbox, FALSE, TRUE, 0);
	gtk_widget_show (hbox);

	StatusInputSizeLabel = gtk_label_new("Input size: 0");
	gtk_box_pack_end (GTK_BOX (hbox), StatusInputSizeLabel, FALSE, TRUE, 2);
	gtk_widget_show (StatusInputSizeLabel);

	/* Add a empty space between input size and the status texts (24 pixels) */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 12);
	gtk_box_pack_start (GTK_BOX (vbox_status), hbox, FALSE, TRUE, 0);
	gtk_widget_show (hbox);

	/* Create the hbox for the lable etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 1);
	gtk_box_pack_start (GTK_BOX (vbox_status), hbox, FALSE, TRUE, 0);
	gtk_widget_show (hbox);

	StatusInputPathLabel = gtk_label_new("/dev/");
	gtk_box_pack_start (GTK_BOX (hbox), StatusInputPathLabel, FALSE, TRUE, 2);
	gtk_widget_show (StatusInputPathLabel);

	/* Create the hbox for the lable etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 1);
	gtk_box_pack_start (GTK_BOX (vbox_status), hbox, FALSE, TRUE, 0);
	gtk_widget_show (hbox);

	// Create the Output path label
	StatusOutputPathLabel = gtk_label_new("/dev/");
	gtk_box_pack_start (GTK_BOX (hbox), StatusOutputPathLabel, FALSE, TRUE, 2);
	gtk_widget_show (StatusOutputPathLabel);

	/* Create the hbox for the lable etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 1);
	gtk_box_pack_start (GTK_BOX (vbox_status), hbox, FALSE, TRUE, 0);
	gtk_widget_show (hbox);

	// Create the Output path label
	StatusBytesReadLabel = gtk_label_new("Bytes read");
	gtk_box_pack_start (GTK_BOX (hbox), StatusBytesReadLabel, FALSE, TRUE, 2);
	gtk_widget_show (StatusBytesReadLabel);


	/* Create the hbox for the lable etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 1);
	gtk_box_pack_start (GTK_BOX (vbox_status), hbox, FALSE, TRUE, 0);
	gtk_widget_show (hbox);

	// Create the Output path label
	StatusBytesWrittenLabel = gtk_label_new("Bytes written");
	gtk_box_pack_start (GTK_BOX (hbox), StatusBytesWrittenLabel, FALSE, TRUE, 2);
	gtk_widget_show (StatusBytesWrittenLabel);

	/* Create the hbox for the lable etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 1);
	gtk_box_pack_start (GTK_BOX (vbox_status), hbox, FALSE, TRUE, 0);
	gtk_widget_show (hbox);

	// Create the Output path label
	StatusAverageSpeedLabel = gtk_label_new("Average speed");
	gtk_box_pack_start (GTK_BOX (hbox), StatusAverageSpeedLabel, FALSE, TRUE, 2);
	gtk_widget_show (StatusAverageSpeedLabel);

	/* Create the align for the status result text */
    align = gtk_alignment_new (0.5, 0.5, 0, 0);
	gtk_box_pack_start (GTK_BOX (vbox_status), align, TRUE, TRUE, 0);
	gtk_widget_show (align);

    // Create the status label (copy done, abort or error)
    StatusResultLabel = gtk_label_new("BUSSY");
    gtk_container_add (GTK_CONTAINER (align), StatusResultLabel);
    gtk_widget_set_name(StatusResultLabel, "StatusLabel");
	gtk_widget_show (StatusResultLabel);


	//================================================================================================
	// Ok, Cancel, Help buttons
	//================================================================================================

	/* Create the hbox for the input radio buttons etc */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_end (GTK_BOX (vbox_main), hbox, FALSE, TRUE, 5);
	gtk_widget_show (hbox);

	// Ok button
	OkeButton = gtk_button_new_from_stock (GTK_STOCK_OK);
	gtk_container_set_border_width (GTK_CONTAINER (OkeButton), 5);
	gtk_box_pack_start (GTK_BOX (hbox), OkeButton, TRUE, TRUE, 0);
	gtk_widget_show (OkeButton);
	g_signal_connect_swapped (G_OBJECT (OkeButton), "event", G_CALLBACK (ok_button_press), G_OBJECT (menu));

	// Cancel button
	CancelButton = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
	gtk_container_set_border_width (GTK_CONTAINER (CancelButton), 5);
	gtk_box_pack_start (GTK_BOX (hbox), CancelButton, TRUE, TRUE, 0);
	gtk_widget_show (CancelButton);
	g_signal_connect_swapped (G_OBJECT (CancelButton), "event", G_CALLBACK (cancel_button_press), G_OBJECT (menu));

	// Help button
	HelpButton = gtk_button_new_from_stock (GTK_STOCK_HELP);
	gtk_container_set_border_width (GTK_CONTAINER (HelpButton), 5);
	gtk_box_pack_start (GTK_BOX (hbox), HelpButton, TRUE, TRUE, 0);
	gtk_widget_show (HelpButton);

	/* And finally we append the menu-item to the menu-bar -- this is the
	* "root" menu-item I have been raving about =) */
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), root_menu);

	// Set the main window size
	//gtk_window_resize ((GtkWindow *)window, 300, 600);
	gtk_window_set_default_size((GtkWindow *)window, 370, 440);
	gtk_widget_set_size_request(window, 370, 440);

	// Don't allow the main window to be resized
	gtk_window_set_resizable ((GtkWindow *)window, false);

	/* always display the window as the last step so it all splashes on
	* the screen at once. */
	gtk_widget_show (window);
//	gtk_widget_show_all (window);

}










