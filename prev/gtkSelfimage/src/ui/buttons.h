#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <gtk/gtk.h>
extern gboolean cancel_button_press( GtkWidget *widget, GdkEvent *event );
extern gboolean ok_button_press( GtkWidget *widget, GdkEvent *event );
extern void OutputFileSelectOkButton( GtkWidget *widget, GtkFileSelection *fs );
extern gboolean ButtonCallback( GtkWidget *widget, gpointer data );
extern gboolean InputRadioButttons_callback( GtkWidget *widget, GdkEvent *event );
extern gboolean OutputRadioButttons_callback( GtkWidget *widget, GdkEvent *event );
extern gboolean CompressionRadioButton_callback( GtkWidget *widget, GdkEvent *event );
#endif
