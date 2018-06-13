/**
 * Project: mbedos_proj
 *
 * Author: Christina Bornberg
 *
 * Date of creation: 18.04.2018
 *
 * File description: Filehandling
 * GtkFileChooserDialog
 *
 * Status: not working
 *
 */

#include <gui_main.h>

/******************************************************************************
 * Start of user functions
 *****************************************************************************/

void file_open(gpointer data) {

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
	    a->file.filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(dialog));
	    file_open ((gpointer) a);
	    g_print("%s\n", a->file.filename);
	    g_free (a->file.filename);
	  }
	gtk_widget_destroy (dialog);
}


