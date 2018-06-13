/**
 * Project: mbedos_proj
 *
 * Author: Christina Bornberg
 *
 * Date of creation: 18.04.2018
 *
 * File description: Main - routine
 *
 * Status:
 *
 */

#include <gui_main.h>

/**
 * Global variables
 */

/******************************************************************************
 * Start of user functions
 *****************************************************************************/

/*
 * The girl loves media technology
 */
void apply_css(GtkWidget *widget, GtkStyleProvider *css_s){
	gtk_style_context_add_provider (gtk_widget_get_style_context (widget), css_s, G_MAXUINT);
	if(GTK_IS_CONTAINER (widget)){
		gtk_container_forall (GTK_CONTAINER (widget), (GtkCallback) apply_css, css_s);
	}
}

void destroy_dialog(GtkDialog *dialog, gint response_id, gpointer data) {
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

void main_gui_menu (gpointer data){
	widgets *a = (widgets *) data;
#if DEBUG
	g_print("Menu, ");
#endif
	menu_init ((gpointer) a);
}

void main_open_file (gpointer data){
	widgets *a = (widgets *) data;
#if DEBUG
		g_print("File, ");
#endif
	file_selection ((gpointer) a);
	file_open ((gpointer) a);
	file_read ((gpointer) a);
}

void main_drawing_area (gpointer data){
	widgets *a = (widgets *) data;
#if DEBUG
		g_print("Draw, ");
#endif
	draw_area_init ((gpointer) a);
}

void main_uart_config (gpointer data){
	widgets *a = (widgets *) data;
	uart_dialog ((gpointer) a);
}

void main_plotter_communication (gpointer data){

}

void activate (GtkApplication *app, gpointer data)
{
	widgets *a = (widgets *) data;

	a->css_style = GTK_STYLE_PROVIDER (gtk_css_provider_new ());
	gtk_css_provider_load_from_resource (GTK_CSS_PROVIDER(a->css_style), "/gui_res/res/style.css");

	// create window and set title *****
	a->window = gtk_application_window_new (a->app);
	gtk_window_set_application (GTK_WINDOW (a->window), GTK_APPLICATION (a->app));
	gtk_window_set_position (GTK_WINDOW (a->window), GTK_WIN_POS_CENTER);
	gtk_window_set_title (GTK_WINDOW (a->window), "Lucas & Chrisy Plotter");
	gtk_window_set_default_size (GTK_WINDOW (a->window), 800, 100); // width, height

	// layout containers *****
	a->main_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add( GTK_CONTAINER (a->window), a->main_box);

	// function calls - all functions in this file
	main_gui_menu ((gpointer) a);
	main_drawing_area ((gpointer) a);

	// css
	apply_css (a->window, a->css_style);

	// visibility - everything is visible
	gtk_widget_show_all (a->window);

}

int main (int argc, char **argv)
{
	int status;

	widgets *a = g_malloc(sizeof(widgets));

	a->app = gtk_application_new ("org.gtk.game", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (a->app, "activate", G_CALLBACK (activate), (gpointer) a);
	status = g_application_run (G_APPLICATION (a->app), argc, argv);
	
	// clean up
#if DEBUG
	g_print("Clean Up \n");
#endif
	g_object_unref (a->app);

	g_free (a);

	return status;
}
