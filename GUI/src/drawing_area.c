/**
 * Project: mbedos_proj
 *
 * Author: Christina Bornberg
 *
 * Date of creation: 06.06.2018
 *
 * File description: Drawing Area
 *
 * Status: beginning
 *
 */

#include <gui_main.h>

/******************************************************************************
 * Start of user functions
 *****************************************************************************/

void file_open(gpointer filename) {

}

void file_read(gpointer data) {

}


void file_selection(gpointer data){

	widgets *a = (widgets *) data;

	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Open File",
	                                      GTK_WINDOW(a->window),
	                                      action,
	                                      "_Cancel",
	                                      GTK_RESPONSE_CANCEL,
	                                      "_Open",
	                                      GTK_RESPONSE_ACCEPT,
	                                      NULL);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	  {
	    gchar *filename;
	    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(dialog));
	    file_open (filename);
	    g_print("%s\n", filename);
	    g_free (filename);
	  }
	gtk_widget_destroy (dialog);
}


