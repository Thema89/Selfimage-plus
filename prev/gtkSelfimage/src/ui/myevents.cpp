#include <stdlib.h>	// atoi ()
#include <gtk/gtk.h>
#include <string.h>
#include <fstream>

void DoEvents (void)
{
      while (gtk_events_pending())
      {
          gtk_main_iteration(); // Handle unprocessed GTK events
      }
}
