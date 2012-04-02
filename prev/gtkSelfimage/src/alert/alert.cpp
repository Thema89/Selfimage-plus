//------------------------------------------------------------------------------------------
//
// Author:	Joris Meijer
// Company:	PasReform
// Date:	07-06-2007
// Version:	0.1
//
//------------------------------------------------------------------------------------------
//
// This file contains a alert function (alike javascript)
//
//------------------------------------------------------------------------------------------
// changes:
//
//
//
//
//------------------------------------------------------------------------------------------
#include <gtk/gtk.h>
#include <string>

#include "alert.h"

gint alert (const std::string Message)
{
gint	result;


	GtkWidget *msgdlg;
	msgdlg = gtk_message_dialog_new(NULL,
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_ERROR,
		GTK_BUTTONS_OK,
		Message.c_str());
	result = gtk_dialog_run(GTK_DIALOG(msgdlg));
	gtk_widget_destroy(msgdlg);

	return result;
}


#if 0
//
// Pops up a alert window
// With sprintf type of input
//
gint alert (const char *format, ...)
{
int	StringSize;
char	*TempString;
gint	result;

	va_list ap;
	va_start(ap, format);

	// Get the size of the string
	StringSize = vsnprintf(0, 0, format, ap) + 2;

	//g_print ("String size = %i\n", StringSize);

	// Allocate the memory for the new string
	TempString = new char[(StringSize + 2)];

	// Copy the new string
	vsnprintf(TempString, StringSize+1, format, ap);
	TempString[StringSize] = '\0';


	GtkWidget *msgdlg;
	msgdlg = gtk_message_dialog_new(NULL,
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_ERROR,
		GTK_BUTTONS_OK,
		TempString);
	result = gtk_dialog_run(GTK_DIALOG(msgdlg));
	gtk_widget_destroy(msgdlg);

	va_end(ap);
	return result;
}
#endif


