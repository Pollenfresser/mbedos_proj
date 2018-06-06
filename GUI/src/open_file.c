/**
 * Project: mbedos_proj
 *
 * Author: Christina Bornberg
 *
 * Date of creation: 18.04.2018
 *
 * File description: Filehandling (open)
 * GtkFileChooserDialog
 *
 * Status: not working
 *
 */

#include <gui_main.h>

/******************************************************************************
 * Start of user functions
 *****************************************************************************/

void open_file_visible(gpointer data){

	widgets *a = (widgets *) data;

	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Open File",
	                                      a->window,
	                                      action,
	                                      _("_Cancel"),
	                                      GTK_RESPONSE_CANCEL,
	                                      _("_Open"),
	                                      GTK_RESPONSE_ACCEPT,
	                                      NULL);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	  {
	    char *filename = "aaaaa";
	    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	    filename = gtk_file_chooser_get_filename (chooser);
	    open_file (filename);
	   // g_free (filename);
	  }

}

void file_selection(gpointer data) {

}



void file_open(gpointer data) {

}

void file_read(gpointer data) {

}
