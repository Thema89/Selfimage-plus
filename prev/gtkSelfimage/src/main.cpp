#include <stdlib.h>	// atoi ()
#include <gtk/gtk.h>
#include <string.h>
#include <fstream>
#include <vector>

#include "./alert/alert.h"
#include "./ui/buttons.h"
#include "./ui/myevents.h"
#include "./ui/main_ui.h"
#include "./DriveList.h"
#include "SelfImage_TImageStore.h"
#include "SelfImage_Utility.h"


TDriveList	*DriveList	= NULL;
TDisplayState	DisplayState;

extern std::string LastInputFileName, LastOutputFileName;

void Initialize (int argc, char *argv[])
{
#ifdef _WIN32
	InitializeUtilityFunctions();
#endif

	DisplayState		= MAIN;
	DriveList		    = new TDriveList;

	/* init threads */
    if (!g_thread_supported ())
	   g_thread_init(NULL);

	/* init gtk */
	gtk_init (&argc, &argv);

    /* Parse rc file */
	gtk_rc_parse("./GTK_Selfimage.rc");

	/* Initialize the ui */
	Initialize_ui ();
}

int main( int argc, char *argv[] )
{
FILE *SettingsFile;
char FileName[255];

    SettingsFile = fopen ("./settings.dat", "r");
    if (SettingsFile!=NULL){
        FileName[0] = '\0';
        fscanf (SettingsFile, "InputFileName = %s\n", FileName);
        LastInputFileName = FileName;
        fscanf (SettingsFile, "OutputFileName = %s\n", FileName);
        LastOutputFileName = FileName;
        fclose (SettingsFile);
    }


	// Initialize the ui
	Initialize ( argc, argv);

	// Call the main gtk func
	gtk_main ();

	return 0;
}







