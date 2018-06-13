/**
 * Project: mbedos_proj
 *
 * Author: Christina Bornberg
 *
 * Date of creation: 11.06.2018
 *
 * File description: Laptop to Plotter via UART
 *
 * Status: not working
 *
 */

#include <gui_main.h>

/******************************************************************************
 * Start of user functions
 *****************************************************************************/


static void on_response (gint response_id, gpointer data);
static void on_entry (gpointer data);

/* The ENTRY key was pressed in the entry box - equivalent to OK
 */
static void
on_entry (gpointer data)
{
	widgets *a = (widgets *) data;
	/* call the callback manually */
	on_response(GTK_RESPONSE_OK, (gpointer) a);
}

/* Callback function in which reacts to the "response" signal from the user in
 * the dialog window.
 */
static void
on_response (gint response_id, gpointer data)
{
	widgets *a = (widgets *) data;

	/* If the button clicked gives response OK (response_id being -5) */
	if (response_id == GTK_RESPONSE_OK)
		gtk_label_set_text (GTK_LABEL(a->uart.label),
				    gtk_entry_get_text (GTK_ENTRY (a->uart.entry)));
	/* If the button clicked gives response CANCEL (response_id being -6) */
	else if (response_id == GTK_RESPONSE_CANCEL)
		gtk_label_set_text (GTK_LABEL(a->uart.label), "Cancel clicked!");
	/* If the message dialog is destroyed (for example by pressing escape) */
	else if (response_id == GTK_RESPONSE_DELETE_EVENT)
		gtk_label_set_text (GTK_LABEL(a->uart.label), "ESC pressed!");

	/* Destroy the dialog after one of the above actions have taken place */
	g_signal_connect(GTK_DIALOG(a->uart.dialog), "response",
				G_CALLBACK(destroy_dialog), (gpointer) a);
	}

void uart_dialog(gpointer data) {

	widgets *a = (widgets *) data;

	GtkWidget *grid;
	GtkWidget *add_button, *cancel_button;
	GtkStyleContext *context;

	GtkWidget *content_area;
	/* Create a new dialog, and set the parameters as follows:
	 * Dialog Flags - make the constructed dialog modal
	 * (modal windows prevent interaction with other windows in the application)
	 * Buttons Type - use the ok and cancel buttons
	 */
	a->uart.dialog = gtk_dialog_new_with_buttons("UART SETTINGS",
			GTK_WINDOW(a->window),
			GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT
					| GTK_DIALOG_USE_HEADER_BAR, GTK_BUTTONS_NONE, NULL);
	add_button = gtk_dialog_add_button(GTK_DIALOG(a->uart.dialog), ("OK"),
			GTK_RESPONSE_OK);
	cancel_button = gtk_dialog_add_button(GTK_DIALOG(a->uart.dialog), ("Cancel"),
			GTK_RESPONSE_CANCEL);
	context = gtk_widget_get_style_context(add_button);
	gtk_style_context_add_class(context, "text-button");
	gtk_style_context_add_class(context, "suggested-action");
	context = gtk_widget_get_style_context(cancel_button);
	gtk_style_context_add_class(context, "text-button");
	gtk_style_context_add_class(context, "destructive-action");
	gtk_window_set_default_size(GTK_WINDOW(a->uart.dialog), 350, 100);

	/* Create a custom dialog below using a label, a text entry and a separator
	 * and attach it to the content area of the dialog.
	 */
	content_area = gtk_dialog_get_content_area(GTK_DIALOG(a->uart.dialog));
	grid = gtk_grid_new();
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
	gtk_container_add(GTK_CONTAINER(content_area), grid);
	gtk_container_set_border_width(GTK_CONTAINER(content_area), 10);
	a->uart.label= gtk_label_new("Enter some text below and click OK ...");
	gtk_grid_attach(GTK_GRID(grid), a->uart.label, 0, 0, 1, 1);
	a->uart.entry = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(grid), a->uart.entry, 0, 1, 1, 1);

	/* display the message dialog */
	gtk_widget_show_all(a->uart.dialog);
	/* connect the response signal */
	g_signal_connect(GTK_DIALOG(a->uart.dialog), "response",
			G_CALLBACK(on_response), a);
	/* connect the ENTER key in the entry box */
	g_signal_connect(GTK_ENTRY(a->uart.entry), "activate", G_CALLBACK(on_entry),
			a);

}


