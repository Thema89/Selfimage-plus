#include <stdlib.h>	// atoi ()
#include <gtk/gtk.h>
#include <string.h>
#include <fstream>

#include "../alert/alert.h"
#include "../DriveList.h"

extern TDriveList	*DriveList;
extern GtkWidget *InputSizeLabel;
extern GtkWidget *OutputSizeLabel;
extern GtkWidget *InputCobobox;
extern GtkWidget *OutputCobobox;


gboolean InputComboChanged( GtkWidget *widget, GdkEvent *event )
{
unsigned int Index;
std::string InputSizeText;

	if (GTK_IS_WIDGET (InputSizeLabel) && GTK_IS_WIDGET (InputCobobox)){
        Index = gtk_combo_box_get_active (GTK_COMBO_BOX(InputCobobox));
		InputSizeText = DriveList->GetDriveSizeString (Index).c_str();
		gtk_label_set_label((GtkLabel *)InputSizeLabel, InputSizeText.c_str());
		return true;
	}
	return false;
}

gboolean OutputComboChanged( GtkWidget *widget, GdkEvent *event )
{
unsigned int Index;
std::string OutputSizeText;
std::string DriveName;

	if (GTK_IS_WIDGET (OutputSizeLabel) && GTK_IS_WIDGET (OutputCobobox)){
        Index = gtk_combo_box_get_active (GTK_COMBO_BOX(OutputCobobox));
		OutputSizeText = DriveList->GetDriveSizeString (Index).c_str();
		gtk_label_set_label((GtkLabel *)OutputSizeLabel, OutputSizeText.c_str());
		return true;
	}
	return false;
}



GtkWidget *CreateInputCombobox ( void )
{
static GtkWidget *Combobox = NULL;
std::string DriveName;
    Combobox = gtk_combo_box_new_text ();

	for (int i = 0; i < DriveList->size(); i++){
        if (DriveList->IsEntireDisk (i)){
            gtk_combo_box_append_text (GTK_COMBO_BOX(Combobox), DriveList->GetDriveName(i).c_str());
        }else{
            DriveName = "    ";
            DriveName += DriveList->GetDriveName(i);
            gtk_combo_box_append_text (GTK_COMBO_BOX(Combobox), DriveName.c_str());
        }
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(Combobox),0);

    g_signal_connect (G_OBJECT (GTK_COMBO_BOX (Combobox)), "changed", G_CALLBACK (InputComboChanged), NULL);

    return Combobox;
}

GtkWidget *CreateOutputCombobox ( void )
{
static GtkWidget *Combobox = NULL;
std::string DriveName;
    Combobox = gtk_combo_box_new_text ();

	for (int i = 0; i < DriveList->size(); i++){
        if (DriveList->IsEntireDisk (i)){
            gtk_combo_box_append_text (GTK_COMBO_BOX(Combobox), DriveList->GetDriveName(i).c_str());
        }else{
            DriveName = "    ";
            DriveName += DriveList->GetDriveName(i);
            gtk_combo_box_append_text (GTK_COMBO_BOX(Combobox), DriveName.c_str());
        }
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(Combobox),0);

    g_signal_connect (G_OBJECT (GTK_COMBO_BOX (Combobox)), "changed", G_CALLBACK (OutputComboChanged), NULL);

    return Combobox;
}

